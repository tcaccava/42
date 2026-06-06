#!/usr/bin/env sh
set -e

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
CERTS_DIR="${ROOT}/infra/certs"
CRT="${CERTS_DIR}/localhost.crt"
KEY="${CERTS_DIR}/localhost.key"

mkdir -p "${CERTS_DIR}"

if [ -f "${CRT}" ] && [ -f "${KEY}" ]; then
  echo "TLS dev certs already exist in ${CERTS_DIR}/ — skipping."
  exit 0
fi

echo "Generating self-signed TLS cert for Docker (localhost) in ${CERTS_DIR}/ ..."

TMP="$(mktemp -d)"
trap 'rm -rf "${TMP}"' EXIT

cat > "${TMP}/openssl.cnf" <<'EOF'
[req]
distinguished_name = req_distinguished_name
x509_extensions = v3_req
prompt = no

[req_distinguished_name]
CN = localhost

[v3_req]
subjectAltName = @alt_names
keyUsage = digitalSignature, keyEncipherment
extendedKeyUsage = serverAuth

[alt_names]
DNS.1 = localhost
IP.1 = 127.0.0.1
EOF

openssl req -x509 -nodes -days 825 -newkey rsa:2048 \
  -keyout "${KEY}" \
  -out "${CRT}" \
  -config "${TMP}/openssl.cnf" \
  -extensions v3_req

chmod 644 "${CRT}"
chmod 600 "${KEY}"
echo "Done. nginx mounts ${CERTS_DIR}/ for make up (HTTPS on APP_HTTPS_PORT, default 8443)."
