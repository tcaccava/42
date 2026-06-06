#!/bin/sh
set -e

echo "==> Applying auth-service migrations..."
cd /repo/apps/auth-service
export DATABASE_URL="${AUTH_DATABASE_URL:?AUTH_DATABASE_URL is required}"
npx prisma migrate deploy

echo "==> Applying user-service migrations..."
cd /repo/apps/user-service
export DATABASE_URL="${USER_DATABASE_URL:?USER_DATABASE_URL is required}"
npx prisma migrate deploy

echo "==> Postgres migrations applied."
