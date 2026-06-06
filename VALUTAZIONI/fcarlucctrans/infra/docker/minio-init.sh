#!/bin/sh
set -e

echo "Waiting for MinIO..."
until mc alias set local "${S3_ENDPOINT}" "${S3_KEY}" "${S3_SECRET}" 2>/dev/null; do
  sleep 1
done

mc mb "local/${S3_BUCKET}" --ignore-existing
mc anonymous set download "local/${S3_BUCKET}" || true
echo "Bucket '${S3_BUCKET}' is ready."
