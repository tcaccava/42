# gateway

Public **HTTP API** for **42 Share**. Single entry point for the Next.js app and external clients. Authenticates requests, sets cookies, and orchestrates microservices.

## Responsibilities

- **42 OAuth** — `/api/auth/login`, `/api/auth/callback`, logout, `/api/auth/profile`
- **Auth guard** — JWT from `access_token` cookie; optional refresh via `refresh_token`
- **Proxy / orchestration**
  - TCP → `auth-service`, `user-service`
  - HTTP → `project-service`, `comments-service`
- **GDPR** — `/api/user/me/export|delete-request|anonymize-request|*-confirm` (cross-service merge)
- **Projects & comments** — search, CRUD, uploads (forwards to backend HTTP services)
- **Users & connections** — profile, search, connection actions
- **Events** — list, create, join, manage
- **Public API** — `/api/v1/resources` with API-key guard

On startup, waits for auth, user, project, and comments TCP/HTTP ports before listening.

## Default URL

- Local: `http://localhost:4000` (`GATEWAY_HOST`, `GATEWAY_PORT`)
- CORS: `WEBAPP_URL` (Next.js origin), `credentials: true`

## Module map

| Module | HTTP prefix | Backend |
|--------|-------------|---------|
| `AuthModule` | `/api/auth/*` | auth-service |
| `UserModule` | `/api/user/*` | user-service |
| `GdprModule` | `/api/user/me/*` | auth + user + project + comments |
| `ProjectModule` | `/projects/*` | project-service |
| `CommentsModule` | `/comments/*` | comments-service |
| `EventsModule` | `/api/events/*` | user-service |
| `PublicApiModule` | `/api/v1/*` | user-service |

`GET /health` — liveness check.

## Environment

Loaded from repo root `.env`. Important keys:

- `CLIENT_ID`, `CLIENT_SECRET`, `REDIRECT_URI` — 42 OAuth
- `JWT_*` — verify access tokens issued by auth-service
- `ACCESS_TOKEN_TTL` — cookie max-age (keep in sync with auth-service)
- `AUTH_SERVICE_*`, `USER_SERVICE_*`, `PROJECT_SERVICE_*`, `COMMENTS_SERVICE_*`
- `WEBAPP_URL`, `NEXT_PUBLIC_API_URL`

## Run locally

```bash
make dev    # from repo root — starts gateway after backends are up
npm run dev -w gateway
```

Gateway blocks until TCP services respond (60s timeout). If login fails with connection errors, ensure auth-service is running on `4001`.

## Layout

```
src/
  auth/           # OAuth, cookies, AuthGuard, JWT refresh
  user/           # Profile, connections, search HTTP
  gdpr/           # Multi-service GDPR orchestration
  project/        # Proxy to project-service
  comments/       # Proxy to comments-service
  events/         # Events HTTP → user-service RPC
  public-api/     # OpenAPI + API key routes
  bootstrap/      # wait-for-tcp, microservice retry
```

## Related docs

- [AUTH.md](../../AUTH.md)
- [GDPR.md](../../GDPR.md)
- [PUBLIC_API.md](../../PUBLIC_API.md)
- [DEPLOYMENT.md](../../DEPLOYMENT.md)
