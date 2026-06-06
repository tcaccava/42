# Public API

REST API to interact with the **user-service** database, secured with an API key and per-key rate limiting.

## Architecture

| Layer | Responsibility |
|-------|----------------|
| **Gateway** (`apps/gateway`) | HTTP routes, `X-API-Key` validation, rate limiting, OpenAPI docs |
| **user-service** (`apps/user-service`) | `ApiKey` + `PublicResource` tables, business logic, persistence |

Cookie/JWT auth is **not** used on these routes — only the API key.

## Setup

```bash
# 1. Apply migrations (includes ApiKey + PublicResource)
npm run db:migrate

# 2. Generate Prisma clients
npm run prisma:generate

# 3. Seed a development API key + sample resources
npm run db:seed:public-api

# 4. Start the stack
npm run dev
```

Copy the printed key into your shell:

```bash
export PUBLIC_API_KEY="ft_dev_…"   # value from seed output
export API_BASE="http://localhost:4000"
```

Default gateway port is **4000** (`GATEWAY_PORT` in `.env`).

---

## Authentication

Send the API key on every **mutating** and **resource read** request:

```http
X-API-Key: <your-key>
```

Alternative:

```http
Authorization: Bearer <your-key>
```

Keys are stored as **SHA-256 hashes** in `user_service.ApiKey` — the raw key is only shown once at creation/seed time.

### Rate limiting

Each key has a `requestsPerMinute` limit (default **60**, dev seed uses **120**).  
When exceeded, the API returns **429** with:

```json
{
  "status": "error",
  "message": "Rate limit exceeded",
  "limit": 120,
  "windowSeconds": 60,
  "retryAfter": 42
}
```

---

## Documentation (no API key)

| Method | Path | Description |
|--------|------|-------------|
| GET | `/api/v1/openapi.json` | OpenAPI 3.0 contract |
| GET | `/api/v1/docs` | Redirects to `openapi.json` |

```bash
curl -s "$API_BASE/api/v1/openapi.json" | head -c 400
```

---

## Resources API (API key required)

Base path: `/api/v1/resources`

| Method | Path | Description |
|--------|------|-------------|
| **GET** | `/api/v1/resources` | List resources (paginated) |
| **GET** | `/api/v1/resources/:id` | Get one resource |
| **POST** | `/api/v1/resources` | Create resource |
| **PUT** | `/api/v1/resources/:id` | Update resource |
| **DELETE** | `/api/v1/resources/:id` | Delete resource |

### GET — list resources

```bash
curl -s "$API_BASE/api/v1/resources?limit=10&offset=0" \
  -H "X-API-Key: $PUBLIC_API_KEY" | jq
```

Example response:

```json
{
  "status": "success",
  "items": [
    {
      "id": "…",
      "title": "Welcome to the Public API",
      "content": "…",
      "tags": ["docs", "seed"],
      "createdAt": "…",
      "updatedAt": "…"
    }
  ],
  "total": 2,
  "limit": 10,
  "offset": 0
}
```

### GET — one resource

```bash
# Replace RESOURCE_ID with an id from the list response
export RESOURCE_ID="00000000-0000-0000-0000-000000000001"

curl -s "$API_BASE/api/v1/resources/$RESOURCE_ID" \
  -H "X-API-Key: $PUBLIC_API_KEY" | jq
```

### POST — create resource

```bash
curl -s -X POST "$API_BASE/api/v1/resources" \
  -H "X-API-Key: $PUBLIC_API_KEY" \
  -H "Content-Type: application/json" \
  -d '{
    "title": "My public note",
    "content": "Created via the Public API",
    "tags": ["demo", "curl"]
  }' | jq
```

Save the returned `item.id` for update/delete examples.

### PUT — update resource

```bash
curl -s -X PUT "$API_BASE/api/v1/resources/$RESOURCE_ID" \
  -H "X-API-Key: $PUBLIC_API_KEY" \
  -H "Content-Type: application/json" \
  -d '{
    "title": "Updated title",
    "content": "Updated body",
    "tags": ["demo", "updated"]
  }' | jq
```

### DELETE — remove resource

```bash
curl -s -X DELETE "$API_BASE/api/v1/resources/$RESOURCE_ID" \
  -H "X-API-Key: $PUBLIC_API_KEY" | jq
```

Example response:

```json
{
  "status": "success",
  "deletedId": "…"
}
```

---

## Error responses

| HTTP | Meaning |
|------|---------|
| 401 | Missing or invalid API key |
| 404 | Resource not found |
| 429 | Rate limit exceeded |
| 400 | Validation error (e.g. missing `title` on POST) |

```bash
# 401 — no key
curl -s -o /dev/null -w "%{http_code}\n" "$API_BASE/api/v1/resources"

# 401 — bad key
curl -s -o /dev/null -w "%{http_code}\n" \
  -H "X-API-Key: invalid" "$API_BASE/api/v1/resources"
```

---

## Database tables (user-service)

### `ApiKey`

| Column | Description |
|--------|-------------|
| `keyHash` | SHA-256 of the raw key (unique) |
| `keyPrefix` | First 12 chars of raw key (for logs) |
| `requestsPerMinute` | Rate limit |
| `isActive` | Disable without deleting |

### `PublicResource`

| Column | Description |
|--------|-------------|
| `title` | Required |
| `content` | Optional text |
| `tags` | String array |

---

## Creating additional API keys

Re-run the seed (replaces the dev key named `Development key`):

```bash
npm run db:seed:public-api
```

Or insert via Prisma Studio after hashing the key with SHA-256:

```bash
cd apps/user-service && npx prisma studio
```

---

## Evaluation checklist

- [x] Secured with API key (`X-API-Key` / Bearer)
- [x] Rate limiting (per key, from DB)
- [x] Documentation (`GET /api/v1/openapi.json`)
- [x] GET `/api/v1/resources`
- [x] POST `/api/v1/resources`
- [x] PUT `/api/v1/resources/:id`
- [x] DELETE `/api/v1/resources/:id`
- [x] Data lives in user-service PostgreSQL
