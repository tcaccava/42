#!/bin/bash
set -e

CONFS_DIR="$(cd "$(dirname "$0")/../confs" && pwd)"
GITLAB_NS="gitlab"
GITLAB_RELEASE="gitlab"
GITLAB_PROJECT="iot-gitops"

# --- Namespace ---
kubectl create namespace "$GITLAB_NS" --dry-run=client -o yaml | kubectl apply -f -

# --- External Postgres/Redis/MinIO ---
# The chart no longer bundles these (removed in chart 10.0 / GitLab 19), so deploy
# standalone instances first and wait for them before installing GitLab itself.
# Credentials are generated here (random on first run) rather than committed to git.
secret_value_or_generate() {
  local secret="$1" key="$2"
  local existing
  existing=$(kubectl get secret "$secret" -n "$GITLAB_NS" -o jsonpath="{.data.$key}" 2>/dev/null | base64 -d 2>/dev/null || true)
  if [ -n "$existing" ]; then
    echo "$existing"
  else
    openssl rand -hex 16
  fi
}

POSTGRES_PASSWORD=$(secret_value_or_generate gitlab-postgres-app password)
REDIS_PASSWORD=$(secret_value_or_generate gitlab-redis-auth password)
MINIO_ROOT_USER="gitlab"
MINIO_ROOT_PASSWORD=$(secret_value_or_generate gitlab-minio-root root-password)

kubectl create secret generic gitlab-postgres-app -n "$GITLAB_NS" \
  --from-literal=password="$POSTGRES_PASSWORD" \
  --dry-run=client -o yaml | kubectl apply -f -

kubectl create secret generic gitlab-redis-auth -n "$GITLAB_NS" \
  --from-literal=password="$REDIS_PASSWORD" \
  --dry-run=client -o yaml | kubectl apply -f -

kubectl create secret generic gitlab-minio-root -n "$GITLAB_NS" \
  --from-literal=root-user="$MINIO_ROOT_USER" \
  --from-literal=root-password="$MINIO_ROOT_PASSWORD" \
  --dry-run=client -o yaml | kubectl apply -f -

# fog-aws connection secret for GitLab's consolidated object storage, pointing at the
# MinIO deployed below.
kubectl create secret generic gitlab-rails-storage -n "$GITLAB_NS" \
  --from-literal=connection="provider: AWS
region: us-east-1
aws_access_key_id: ${MINIO_ROOT_USER}
aws_secret_access_key: ${MINIO_ROOT_PASSWORD}
endpoint: http://gitlab-minio.gitlab.svc.cluster.local:9000
path_style: true" \
  --dry-run=client -o yaml | kubectl apply -f -

# s3cmd config for the toolbox's separate backup/restore mechanism (gitlab.toolbox.backups).
kubectl create secret generic gitlab-backup-s3cfg -n "$GITLAB_NS" \
  --from-literal=config="[default]
access_key = ${MINIO_ROOT_USER}
secret_key = ${MINIO_ROOT_PASSWORD}
bucket_location = us-east-1
host_base = gitlab-minio.gitlab.svc.cluster.local:9000
host_bucket = gitlab-minio.gitlab.svc.cluster.local:9000
use_https = False
signature_v2 = False" \
  --dry-run=client -o yaml | kubectl apply -f -

kubectl apply -f "$CONFS_DIR/postgres.yaml"
kubectl apply -f "$CONFS_DIR/redis.yaml"
kubectl apply -f "$CONFS_DIR/minio.yaml"

echo "Waiting for Postgres, Redis and MinIO to be ready..."
kubectl rollout status deployment/gitlab-postgresql -n "$GITLAB_NS" --timeout=180s
kubectl rollout status deployment/gitlab-redis -n "$GITLAB_NS" --timeout=180s
kubectl rollout status deployment/gitlab-minio -n "$GITLAB_NS" --timeout=180s
kubectl wait --for=condition=complete --timeout=180s job/gitlab-minio-create-buckets -n "$GITLAB_NS"

# --- Install GitLab via Helm ---
helm repo add gitlab https://charts.gitlab.io/ >/dev/null 2>&1 || true
helm repo update gitlab

echo "Installing GitLab via Helm, it can take a while"
helm upgrade --install "$GITLAB_RELEASE" gitlab/gitlab \
  -n "$GITLAB_NS" \
  -f "$CONFS_DIR/gitlab-values.yaml" \
  --timeout 1200s \
  --wait

echo "Waiting for the GitLab toolbox (used to run gitlab-rails commands)..."
kubectl rollout status deployment/"${GITLAB_RELEASE}"-toolbox -n "$GITLAB_NS" --timeout=600s
TOOLBOX_POD=$(kubectl get pod -n "$GITLAB_NS" -l app=toolbox -o jsonpath='{.items[0].metadata.name}')

echo "Fetching the initial root password..."
ROOT_PASSWORD=$(kubectl get secret -n "$GITLAB_NS" "${GITLAB_RELEASE}-gitlab-initial-root-password" -o jsonpath='{.data.password}' | base64 -d)

# --- Bootstrap: a Personal Access Token for root, used to create the project and push ---
echo "Generating a Personal Access Token for root..."
PAT=$(kubectl exec -n "$GITLAB_NS" "$TOOLBOX_POD" -- gitlab-rails runner "
  token = User.find_by_username('root').personal_access_tokens.create(
    scopes: [:api, :write_repository],
    name: 'bootstrap',
    expires_at: 365.days.from_now
  )
  token.set_token('bootstrap-' + SecureRandom.hex(10))
  token.save!
  puts token.token
" | tail -1)

echo "Creating the '$GITLAB_PROJECT' project..."
# "localhost" is wrong here: this curl runs inside the toolbox pod (via kubectl exec), a
# separate pod from webservice/workhorse, so it must target the Service DNS name instead
# (same host used for the repoURL below, not the host-side port-forward used after this).
kubectl exec -n "$GITLAB_NS" "$TOOLBOX_POD" -- curl --silent --request POST \
  --header "PRIVATE-TOKEN: $PAT" \
  --data "name=$GITLAB_PROJECT&visibility=public" \
  "http://${GITLAB_RELEASE}-webservice-default.${GITLAB_NS}.svc:8181/api/v4/projects" > /dev/null

# --- Push the same manifests used in p3 into the new GitLab project ---
echo "Port-forwarding GitLab's webservice locally to push the initial manifests..."
kubectl port-forward -n "$GITLAB_NS" "svc/${GITLAB_RELEASE}-webservice-default" 8181:8181 &
PF_PID=$!
trap 'kill "$PF_PID" 2>/dev/null || true' EXIT
sleep 5

WORKDIR=$(mktemp -d)
git clone "http://root:${PAT}@localhost:8181/root/${GITLAB_PROJECT}.git" "$WORKDIR"
# Download straight into the repo's manifests/ dir (Argo CD's source.path is "manifests")
mkdir -p "$WORKDIR/manifests"
wget -O "$WORKDIR/manifests/deployment.yaml" https://raw.githubusercontent.com/Shadowaker/dridolfo-cd/refs/heads/master/manifests/deployment.yaml
(
  cd "$WORKDIR"
  git add manifests
  git -c user.name="root" -c user.email="root@gitlab.local" commit -m "Initial manifests"
  git push origin HEAD:master
)
rm -rf "$WORKDIR"

kill "$PF_PID" 2>/dev/null || true
trap - EXIT

# --- Register the local GitLab repo with Argo CD, then re-point iot-app at it ---
echo "Registering the local GitLab repo credentials with Argo CD..."
kubectl apply -n argocd -f - <<EOF
apiVersion: v1
kind: Secret
metadata:
  name: gitlab-repo
  namespace: argocd
  labels:
    argocd.argoproj.io/secret-type: repository
stringData:
  type: git
  url: http://${GITLAB_RELEASE}-webservice-default.${GITLAB_NS}.svc:8181/root/${GITLAB_PROJECT}.git
  username: root
  password: "${PAT}"
EOF

echo "Re-pointing the iot-app Argo CD Application at local GitLab..."
kubectl apply -f "$CONFS_DIR/argocd-app-gitlab.yaml"

echo
echo "Bonus setup complete."
echo "GitLab root password: $ROOT_PASSWORD"
echo "GitLab UI: kubectl port-forward -n gitlab svc/${GITLAB_RELEASE}-webservice-default 8181:8181  then browse http://localhost:8181"
echo "Verify sync: kubectl get application -n argocd iot-app"
