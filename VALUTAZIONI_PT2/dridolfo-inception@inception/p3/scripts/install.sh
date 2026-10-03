#!/bin/bash
set -e

# curl
if ! command -v curl &> /dev/null; then
  sudo apt-get update -qq
  sudo apt-get install -y -qq curl
fi

# Docker
if ! command -v docker &> /dev/null; then
  curl -fsSL https://get.docker.com | sh
  sudo usermod -aG docker "$USER"
fi

# To make the docker usable by user
sudo usermod -aG docker "$USER"

# kubectl
if ! command -v kubectl &> /dev/null; then
  KUBECTL_VERSION=$(curl -L -s https://dl.k8s.io/release/stable.txt)
  curl -LO "https://dl.k8s.io/release/${KUBECTL_VERSION}/bin/linux/amd64/kubectl"
  sudo install -o root -g root -m 0755 kubectl /usr/local/bin/kubectl
  rm -f kubectl
fi

# K3d
if ! command -v k3d &> /dev/null; then
  curl -s https://raw.githubusercontent.com/k3d-io/k3d/main/install.sh | bash
fi

# Argo CD CLI
if ! command -v argocd &> /dev/null; then
  curl -sSL -o argocd-linux-amd64 https://github.com/argoproj/argo-cd/releases/latest/download/argocd-linux-amd64
  sudo install -m 0555 argocd-linux-amd64 /usr/local/bin/argocd
  rm -f argocd-linux-amd64
fi

echo "Done."
