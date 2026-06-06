# Database UI (Prisma Studio)

One **PostgreSQL** instance, **two databases** (database-per-service):

| Database | Service | Prisma schema |
|----------|---------|----------------|
| `auth_service` | auth-service | `apps/auth-service/prisma` |
| `user_service` | user-service | `apps/user-service/prisma` |

Each DB uses the default PostgreSQL schema `public`.

## Docker (starts with `make up`)

| UI | URL (default) |
|----|-----|
| **auth-service** models | http://localhost:5565 |
| **user-service** models | http://localhost:5566 |

Ports: `PRISMA_STUDIO_AUTH_PORT` / `PRISMA_STUDIO_USER_PORT` in `.env.docker`.

### Port already in use (`bind: address already in use`)

Local `npx prisma studio` uses **5555** by default. Docker defaults to **5565/5566** to avoid that clash.

Either stop the local studio (`Ctrl+C` in that terminal), or set free ports in `.env.docker`:

```env
PRISMA_STUDIO_AUTH_PORT=5565
PRISMA_STUDIO_USER_PORT=5566
```

Find what uses a port: `lsof -i :5555`

Prisma Studio shows your **Prisma models** (tables, relations, edit rows) — the same UI as `npx prisma studio` in each app.

## Local dev (without Docker)

```bash
# Terminal 1 — auth DB
cd apps/auth-service
DATABASE_URL="postgresql://postgres:postgres@localhost:5432/auth_service?schema=public" npx prisma studio

# Terminal 2 — user DB
cd apps/user-service
DATABASE_URL="postgresql://postgres:postgres@localhost:5432/user_service?schema=public" npx prisma studio
```

Default local port is **5555**; the second instance picks **5556** automatically if 5555 is taken.

## External clients (DBeaver, TablePlus, `psql`)

```text
postgresql://postgres:postgres@localhost:5432/auth_service?schema=public
postgresql://postgres:postgres@localhost:5432/user_service?schema=public
```

(Postgres on host when using `make dev-db` or `infra/db/docker-compose.yml`.)
