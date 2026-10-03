#!/bin/bash
set -e

# curl
if ! command -v curl &> /dev/null; then
  sudo apt-get update -qq
  sudo apt-get install -y -qq curl
fi

# openssl (needed by setup.sh to generate random credentials for Postgres/Redis/MinIO)
if ! command -v openssl &> /dev/null; then
  sudo apt-get update -qq
  sudo apt-get install -y -qq openssl
fi

# Helm (needed to install the GitLab chart)
if ! command -v helm &> /dev/null; then
  curl -fsSL -o get_helm.sh https://raw.githubusercontent.com/helm/helm/main/scripts/get-helm-3
  chmod +x get_helm.sh
  sudo ./get_helm.sh
  rm -f get_helm.sh
fi

echo "install.sh done. Assumes docker/kubectl/k3d/argocd CLI are already installed (see p3/scripts/install.sh)."
