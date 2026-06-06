# web

Next.js **frontend** for **42 Share** — the 42 student project-sharing platform (feed, profiles, projects, events, settings).

## Stack

- **Next.js 16** (App Router), React 19, TypeScript
- **Tailwind CSS 4** — layout, dark mode, high-contrast tokens in `globals.css`
- **Server Actions** + `fetchWithAuth` — calls gateway with session cookies
- **i18n** — locale JSON under `src/locales/` (en, fr, de, it, uk)

## Routes

| Path | Purpose |
|------|---------|
| `/` | Landing + 42 login |
| `/feed` | Project feed (infinite scroll) |
| `/search` | People + project search |
| `/connections` | Pending and accepted connections |
| `/create` | Multi-step project publish wizard |
| `/create/edit` | Edit own project |
| `/project/[id]` | Project detail, screenshots, comments |
| `/profile`, `/profile/[login]` | Own or public profile |
| `/events`, `/events/create`, `/events/manage` | Campus events |
| `/settings` | Theme, language, display |
| `/settings/privacy` | GDPR export / delete / anonymize |
| `/settings/privacy/confirm/[type]` | Email token confirmation (no auth required) |

Platform pages use `(platform)/layout.tsx`: header, sidebar, authenticated shell.

## API integration

- Browser and Server Components talk to **`NEXT_PUBLIC_API_URL`** (gateway, default `http://localhost:4000`)
- Session: `access_token` + `refresh_token` httpOnly cookies set by gateway OAuth callback
- Logout: `/api/logout` (Next route) → gateway `/api/auth/logout`

Key server modules:

```
src/server/
  auth.ts       # Profile fetch
  project.ts    # Projects search, CRUD
  gdpr.ts       # Privacy actions
  events.ts     # Events
  users.ts      # Connections, public profiles
  http.ts       # fetchWithAuth, cookie forwarding
```

## Features (UI)

- **Projects** — 42 subject select + public GitHub/GitLab repo URL, screenshots, source archive (S3 via gateway)
- **Comments** — per-project threads
- **Connections** — request / accept flow with active status badge
- **Settings** — theme, high contrast, language; privacy panel with GDPR actions
- **Accessibility** — skip link, semantic tokens, localized strings

## Run locally

```bash
# From repo root (recommended)
make dev

# Web only (gateway must already run on :4000)
npm run dev -w web
```

Default: **http://localhost:3000**

Env (repo root `.env`):

- `NEXT_PUBLIC_API_URL=http://localhost:4000`
- `WEBAPP_URL=http://localhost:3000`

## Layout

```
src/
  app/              # App Router pages + layouts
  components/       # UI (feed, search, settings, forms, …)
  server/           # Server Actions → gateway
  utilities/        # i18n, appearance (theme boot)
  locales/          # Translation files
  types/            # Shared TS types
```
