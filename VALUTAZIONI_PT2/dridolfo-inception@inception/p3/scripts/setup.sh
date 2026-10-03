#!/bin/bash
set -e

CONFS_DIR="$(cd "$(dirname "$0")/../confs" && pwd)"

# Create the K3d cluster, publishing the app's NodePort (30888) on host port 8888
k3d cluster create iot --port "8888:30888@server:0" --wait

kubectl apply -f "$CONFS_DIR/namespace.yaml"

# Install Argo CD into the argocd namespace
# --server-side avoids kubectl's client-side "last-applied-configuration" annotation,
# which overflows Kubernetes' 256KiB annotation limit on the large applicationsets.argoproj.io CRD.
kubectl apply -n argocd --server-side --force-conflicts -f https://raw.githubusercontent.com/argoproj/argo-cd/stable/manifests/install.yaml
kubectl wait --for=condition=available deployment/argocd-server -n argocd --timeout=300s

# Wait for the Application CRD to be registered before applying our Application
kubectl wait --for=condition=established --timeout=60s crd/applications.argoproj.io

# Register the GitOps Application (auto-sync onto the dev namespace)
kubectl apply -f "$CONFS_DIR/argocd-app.yaml"

echo "Setup complete."
echo
echo "Argo CD initial admin password:"
kubectl -n argocd get secret argocd-initial-admin-secret -o jsonpath="{.data.password}" | base64 -d
echo ""

# "To view the Argo CD UI: kubectl port-forward -n argocd svc/argocd-server 8080:443"
# "Then browse to https://localhost:8080 (user: admin)"
# "App: curl http://localhost:8888/"
#  Per il port-forward: kubectl port-forward -n argocd svc/argocd-server 8080:443

