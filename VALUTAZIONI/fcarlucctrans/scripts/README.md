# Scripts — setup & testing

Helper scripts for **local setup** and **manual testing**. None of them run in production.

Run all commands from the **repo root**.

---

## Quick start — one command

**Docker (full stack running):**

```bash
make seed
```

**Local dev:**

```bash
make dev-setup          # first time only
make seed-local         # loads .env, generates Prisma clients, seeds everything
```

**Or via npm:**

```bash
npm run prisma:generate
npm run db:seed         # all demo data
npm run db:seed:reset   # wipe fixtures, then re-seed
```

---

## What `db:seed` populates

| Step | Database | Content |
|------|----------|---------|
| Users | auth + user | 8 fake students, sessions, connections, profile **cover images** |
| Events | user | 4 campus events with **cover images** + participants |
| Projects | project (SQLite) | **15 projects** for search pagination + optional S3 screenshots |
| Public API | user | Dev API key + sample resources (printed to terminal) |

All avatars and covers use **Unsplash JPEG URLs** (allowed in `next.config.ts`).

Project screenshots are uploaded to MinIO/S3 when `S3_*` env vars are set. Use `--skip-s3` to skip uploads.

---

## Individual scripts

| Script | Command |
|--------|---------|
| `seed-all.mjs` | `npm run db:seed` |
| `seed-databases.mjs` | `npm run db:seed:users` |
| `seed-events.mjs` | `npm run db:seed:events` |
| `seed-projects.mjs` | `npm run db:seed:projects` |
| `seed-public-api.mjs` | `npm run db:seed:public-api` |
| `seed-pending-invitations.mjs` | `npm run db:seed:invitations -- <login>` |

```bash
npm run db:seed:invitations -- your_42_login   # after signing in once
npm run db:seed:public-api                     # Public API only
npm run db:seed:projects -- --skip-s3          # projects without MinIO
```

---

## Prerequisites

- Postgres running (`make dev-db` or `make up`)
- Migrations applied (`make dev-migrate` or Docker `migrate` service)
- `npm run prisma:generate` (included in `make seed-local`)
- For project screenshots: MinIO/S3 up (`S3_ENDPOINT` in `.env`)

---

## Seed users (search & profiles)

| Login | Name | Campus |
|-------|------|--------|
| `jdoe` | John Doe | 42 Paris |
| `asmith` | Alice Smith | 42 Rome |
| `bwilson` | Bob Wilson | 42 Berlin |
| `cgarcia` | Carlos Garcia | 42 Barcelona |
| `dyoon` | Diana Yoon | 42 London |
| `epark` | Ethan Park | 42 Heilbronn |
| `frossi` | Francesca Rossi | 42 Florence |
| `gkim` | George Kim | 42 Paris |

Fake users **cannot log in** via OAuth — they exist for search, profiles, connections, and demo projects.

---

## Testing after seed

1. `make dev` or `make up`
2. Sign in with your real 42 account
3. **Search** → students, events, paginated projects
4. **Events** → 4 seeded campus events with images
5. Copy printed `PUBLIC_API_KEY` into `.env` for Public API tests ([PUBLIC_API.md](../PUBLIC_API.md))

---

## Troubleshooting

| Problem | Check |
|---------|--------|
| `Projects seed failed` (Prisma client) | Run `npm run prisma:generate` |
| No project screenshots | MinIO running? Or use `--skip-s3` |
| `db:seed` connection refused | Postgres up? `make dev-db` |
| Invitations script fails | Sign in once so your login exists |

Reset all fixtures: `npm run db:seed:reset`
