# user-service

Social and platform data backend for **42 Share**. Runs as a **TCP microservice** (no public HTTP). The gateway forwards HTTP requests as RPC commands.

## Responsibilities

- **Profiles** — bio, links, campus, cover image (`UserProfile`, keyed by JWT `sub`)
- **Connections** — send, accept, reject, cancel, remove; pending/accepted lists
- **Events** — campus events CRUD, join/leave, participant caps
- **Public API storage** — API keys (hashed), generic `PublicResource` CRUD for `/api/v1/resources`
- **GDPR (user slice)** — export profile/connections/events; delete or anonymize user-owned rows

## Database

| Engine | Prisma schema |
|--------|----------------|
| PostgreSQL | `prisma/schema.prisma` |

Main models: `UserProfile`, `Connection`, `Event`, `EventParticipant`, `ApiKey`, `PublicResource`.

Env: `USER_DATABASE_URL` in repo root `.env`.

## TCP message patterns

| Area | Commands (examples) |
|------|------------------------|
| Profile | `user_profile_get`, `user_profile_upsert` |
| Connections | `user_connection_send`, `_accept`, `_reject`, `_cancel`, `_remove`, `_list`, `_status`, `_count` |
| Events | `event_list`, `event_get`, `event_create`, `event_update`, `event_delete`, `event_join`, `event_leave` |
| Public API | `public_api_validate_key`, `public_resource_*` |
| GDPR | `gdpr_export_user`, `gdpr_delete_user`, `gdpr_anonymize_user` |

## Run locally

Default bind: `USER_SERVICE_HOST=127.0.0.1`, `USER_SERVICE_PORT=4002`.

```bash
make dev-setup          # Postgres + migrations (first time)
npm run dev -w user-service

npm run prisma:migrate -w user-service
npm run db:seed          # from repo root — sample students + connections
```

## Layout

```
src/
  app.controller.ts       # TCP handlers
  app.service.ts          # Profile CRUD
  connection.service.ts   # Friend requests
  event.service.ts        # Events + participants
  public-api/             # API key + resources
  gdpr/                   # User-data export/purge
  database/               # Prisma repositories
prisma/schema.prisma
```

## Related docs

- [PUBLIC_API.md](../../PUBLIC_API.md) — gateway REST surface for resources
- [GDPR.md](../../GDPR.md) — full export/delete orchestration
- [scripts/README.md](../../scripts/README.md) — seed data
