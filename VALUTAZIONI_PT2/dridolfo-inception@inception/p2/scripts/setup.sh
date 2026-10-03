#!/bin/bash
set -e

apt-get update -qq
apt-get install -y -qq curl
apt-get install -y net-tools

# Install K3s in server mode (Traefik stays enabled: it's our Ingress controller)
curl -sfL https://get.k3s.io | INSTALL_K3S_EXEC="server --write-kubeconfig-mode=644" sh -

# Make kubectl available globally
echo 'export KUBECONFIG=/etc/rancher/k3s/k3s.yaml' >> /etc/profile.d/k3s.sh
chmod +x /etc/profile.d/k3s.sh
export KUBECONFIG=/etc/rancher/k3s/k3s.yaml

# Wait for K3s to be ready
until kubectl get nodes 2>/dev/null | grep -q "Ready"; do
  echo "Waiting for K3s server to be ready..."
  sleep 3
done

# Wait for Traefik's CRDs/deployment to be installed before applying the Ingress
until kubectl get deployment traefik -n kube-system >/dev/null 2>&1; do
  echo "Waiting for Traefik to be installed..."
  sleep 3
done
kubectl rollout status deployment/traefik -n kube-system --timeout=180s

kubectl apply -f /vagrant/confs/app1.yaml
kubectl apply -f /vagrant/confs/app2.yaml
kubectl apply -f /vagrant/confs/app3.yaml
kubectl apply -f /vagrant/confs/ingress.yaml

echo "K3s server ready. Apps and Ingress applied."
kubectl get all
