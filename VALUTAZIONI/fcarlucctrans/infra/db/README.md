# Postgres only (local dev helper)

This compose file starts **only PostgreSQL**, not the full application.

## Full stack (evaluation / production)

From the **repo root**:

```bash
make up
# or
docker compose up --build -d
```

App URL: http://localhost:8080 — see [DEPLOYMENT.md](../../DEPLOYMENT.md).

## Browse databases (Prisma Studio)

Included in `make up` → :5555 (auth) and :5556 (user) — see [MONITORING.md](MONITORING.md).

## Postgres only (optional, for `npm run dev` on the host)

```bash
docker compose -f infra/db/docker-compose.yml up -d
npm run db:migrate
npm run dev
```
