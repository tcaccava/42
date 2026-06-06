#!/usr/bin/env sh
set -e

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
KEYS_DIR="${ROOT}/keys"

mkdir -p "${KEYS_DIR}"

if [ -f "${KEYS_DIR}/jwt_private.pem" ] && [ -f "${KEYS_DIR}/jwt_public.pem" ]; then
  echo "JWT keys already exist in ${KEYS_DIR}/ — skipping."
  exit 0
fi

echo "Generating RSA 4096-bit JWT key pair in ${KEYS_DIR}/ ..."
openssl genrsa -out "${KEYS_DIR}/jwt_private.pem" 4096
openssl rsa -in "${KEYS_DIR}/jwt_private.pem" -pubout -out "${KEYS_DIR}/jwt_public.pem"
chmod 600 "${KEYS_DIR}/jwt_private.pem"
chmod 644 "${KEYS_DIR}/jwt_public.pem"
echo "Done. Mount these in Docker via ./keys:/keys:ro"
