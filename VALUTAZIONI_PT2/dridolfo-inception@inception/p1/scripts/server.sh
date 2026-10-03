#!/bin/bash
set -e

apt-get update -qq
apt-get install -y -qq curl
apt-get install -y net-tools

# Install K3s in server in controller mode
curl -sfL https://get.k3s.io | INSTALL_K3S_EXEC="server --write-kubeconfig-mode=644 --disable=traefik --disable=servicelb --disable=metrics-server" sh -

# Wait for K3s to be ready
until kubectl get nodes 2>/dev/null | grep -q "Ready"; do
  echo "Waiting for K3s server to be ready..."
  sleep 3
done

# Share the node token with the worker via the /vagrant shared folder
cp /var/lib/rancher/k3s/server/node-token /vagrant/node-token

# Make kubectl available globally
echo 'export KUBECONFIG=/etc/rancher/k3s/k3s.yaml' >> /etc/profile.d/k3s.sh
chmod +x /etc/profile.d/k3s.sh

echo "K3s server ready. Node token exported."
kubectl get nodes -o wide
