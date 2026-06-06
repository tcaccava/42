#!/bin/sh
set -e

echo "==> Initializing comments-service SQLite database..."
cd /repo/apps/comments-service
export DATABASE_URL="${COMMENTS_DATABASE_URL:?COMMENTS_DATABASE_URL is required}"
npx prisma migrate deploy

echo "==> Initializing project-service SQLite database..."
cd /repo/apps/project-service
export DATABASE_URL="${PROJECT_DATABASE_URL:?PROJECT_DATABASE_URL is required}"
npx prisma db push --accept-data-loss

echo "==> SQLite databases ready."
