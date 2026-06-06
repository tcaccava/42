# auth-service

Identity and session backend for **42 Share**. Runs as a **TCP microservice** (no public HTTP). The gateway is the only caller.

## Responsibilities

- **42 OAuth** — exchange authorization code for Intra profile, upsert local `User`
- **JWT (RS256)** — issue access tokens; verify tokens and session validity
- **Refresh sessions** — rotating refresh tokens stored in Postgres
- **User directory** — search/list 42 users for the platform (login, campus, active status)
- **GDPR (auth slice)** — export auth data, email-driven delete/anonymize tokens, account scrubbing on confirm

## Database

| Engine | Prisma schema |
|--------|----------------|
| PostgreSQL | `prisma/schema.prisma` |

Main models: `User`, `UserSession`, `RefreshSession`, `DataRequest`.

Env: `AUTH_DATABASE_URL` (repo root `.env`) or `DATABASE_URL` in `prisma/.env`.

## TCP message patterns

| Command | Purpose |
|---------|---------|
| `validate_42_user` | OAuth code → user + tokens |
| `verify_jwt` | Validate access JWT |
| `refresh_token` | Rotate refresh → new access |
| `session_validate` / `session_revoke` | Session lifecycle |
| `user_is_active` | Online badge (session not expired) |
| `user_directory` / `user_search` / `user_find_by_login` | People search |
| `gdpr_*` | Export, delete/anonymize request & confirm |

## Email (GDPR)

`src/email/mailer.service.ts` sends confirmation mail via **Gmail API (HTTPS)** when `MAIL_*` is set, or logs to stdout with `MAIL_FORCE_FALLBACK=true`. See [GDPR.md](../../GDPR.md).

## Run locally

Started by Turbo from the repo root (`make dev`). Default bind:

- `AUTH_SERVICE_HOST=127.0.0.1`
- `AUTH_SERVICE_PORT=4001`

```bash
# From repo root (after make dev-setup)
npm run dev -w auth-service

# Migrations
npm run prisma:migrate -w auth-service
```

## Layout

```
src/
  app.controller.ts      # TCP handlers
  app.service.ts         # OAuth, JWT, sessions, user lookup
  gdpr/                  # GDPR flows + token verification
  email/                 # Mailer + templates
  database/              # Prisma repositories
prisma/schema.prisma
```

## Related docs

- [AUTH.md](../../AUTH.md) — cookies, JWT TTL, refresh rotation
- [GDPR.md](../../GDPR.md) — privacy flows orchestrated via gateway
