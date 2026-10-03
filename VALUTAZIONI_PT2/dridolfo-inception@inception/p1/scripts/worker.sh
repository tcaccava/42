#!/bin/bash
set -e

apt-get update -qq
apt-get install -y -qq curl

# Wait for the server to export the node token
until [ -f /vagrant/node-token ]; do
  echo "Waiting for K3s server token..."
  sleep 3
done

TOKEN=$(cat /vagrant/node-token)

# Install K3s in agent mode, joining the server
curl -sfL https://get.k3s.io | \
  K3S_URL="https://192.168.56.110:6443" \
  K3S_TOKEN="$TOKEN" \
  sh -

echo "K3s agent ready and joined the cluster."
