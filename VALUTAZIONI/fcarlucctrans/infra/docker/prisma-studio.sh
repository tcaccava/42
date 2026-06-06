#!/bin/sh
# POSIX sh — do not use `wait -n` (bash-only); it crashes the container on Debian.

AUTH_DB="${AUTH_DATABASE_URL:?}"
USER_DB="${USER_DATABASE_URL:?}"
AUTH_PORT="${PRISMA_STUDIO_AUTH_PORT:-5555}"
USER_PORT="${PRISMA_STUDIO_USER_PORT:-5556}"

echo "Prisma Studio — auth_service:  http://0.0.0.0:${AUTH_PORT}"
echo "Prisma Studio — user_service: http://0.0.0.0:${USER_PORT}"

cd /repo/apps/auth-service
export DATABASE_URL="${AUTH_DB}"
npx prisma studio --port "${AUTH_PORT}" --hostname 0.0.0.0 --browser none &
PID_AUTH=$!

cd /repo/apps/user-service
export DATABASE_URL="${USER_DB}"
npx prisma studio --port "${USER_PORT}" --hostname 0.0.0.0 --browser none &
PID_USER=$!

# Keep container alive while both studios run
wait "$PID_AUTH" "$PID_USER"
