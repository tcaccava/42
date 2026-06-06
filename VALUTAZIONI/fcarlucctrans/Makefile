# ft_transcendence — single-command deployment
#
#   make up      # HTTPS via nginx on APP_HTTPS_PORT (default 8443)
#   make dev     # same HTTPS URL; turbo on :3000/:4000 behind dev nginx proxy

.PHONY: up up-single down logs seed seed-local rebuild ps dev dev-clean dev-db dev-migrate dev-setup env-check prepare certs dev-proxy

COMPOSE := docker compose --env-file .env --env-file .env.docker
DEV_PROXY := docker compose -f infra/nginx/docker-compose.dev-proxy.yml --env-file .env

# Stop stale local dev servers (orphans after Ctrl+C or crashed turbo)
dev-clean:
	@echo "Stopping stale dev processes on :3000, :4000-:4004, dev HTTPS proxy..."
	@$(DEV_PROXY) down --remove-orphans 2>/dev/null || true
	@for port in 3000 4000 4001 4002 4003 4004 8443; do \
		pid=$$(lsof -tiTCP:$$port -sTCP:LISTEN 2>/dev/null); \
		if [ -n "$$pid" ]; then kill $$pid 2>/dev/null && echo "  freed :$$port (pid $$pid)"; fi; \
	done
	@pkill -f '42cursus-ft_transcendence.*nest start' 2>/dev/null || true
	@pkill -f '42cursus-ft_transcendence.*next dev' 2>/dev/null || true

# Alias — same file prep as `make up`, without starting containers
prepare: .env .env.docker keys certs
	@echo "Ready (.env, .env.docker, keys, TLS certs). Run: make up"

# Deprecated alias — use `make up`
setup: prepare
	@echo "(setup is optional — make up does this automatically)"

.env:
	@test -f .env || (cp .env.example .env && echo "Created .env")

.env.docker:
	@test -f .env.docker || (cp .env.docker.example .env.docker && echo "Created .env.docker")

keys:
	@chmod +x scripts/generate-jwt-keys.sh
	@./scripts/generate-jwt-keys.sh

certs:
	@chmod +x scripts/generate-dev-certs.sh
	@./scripts/generate-dev-certs.sh

env-check:
	@echo "=== App URLs (.env — shared by make dev and make up) ==="
	@grep -E '^(APP_HTTPS_PORT|WEBAPP_URL|NEXT_PUBLIC_API_URL|REDIRECT_URI)=' .env 2>/dev/null || true

# Docker stack — HTTPS entry via nginx (web/gateway are internal only)
up: .env .env.docker keys certs env-check
	$(COMPOSE) up --build -d
	@echo ""
	@set -a && . ./.env && set +a; \
	port=$${APP_HTTPS_PORT:-8443}; \
	echo "App (HTTPS):  $${WEBAPP_URL:-https://localhost:$$port}"; \
	echo "  (self-signed cert — accept the browser warning on first visit)"; \
	echo "42 OAuth redirect: $${REDIRECT_URI:-https://localhost:$$port/api/auth/callback}"; \
	echo "Prisma Studio (auth):  http://localhost:$${PRISMA_STUDIO_AUTH_PORT:-5565}"; \
	echo "Prisma Studio (user):  http://localhost:$${PRISMA_STUDIO_USER_PORT:-5566}"; \
	echo "MinIO console:         http://localhost:$${MINIO_CONSOLE_PORT:-9001}"

# Deprecated alias — same as make up
up-single: up

down:
	$(COMPOSE) down --remove-orphans

logs:
	$(COMPOSE) logs -f

ps:
	$(COMPOSE) ps

rebuild:
	$(COMPOSE) up --build -d --force-recreate

seed: .env .env.docker
	$(COMPOSE) --profile tools run --rm seed

# Local one-shot demo data (Postgres + SQLite + optional MinIO from .env)
seed-local: .env
	set -a && . ./.env && set +a && npm run prisma:generate && npm run db:seed

dev-db: .env
	docker compose -f infra/db/docker-compose.yml --env-file .env up -d

# Apply Prisma schema (Postgres + SQLite — required once after dev-db / fresh volume)
dev-migrate: .env
	set -a && . ./.env && set +a && npm run prisma:generate && npm run db:migrate
	@cd apps/project-service && PROJECT_DATABASE_URL="file:./dev.db" npx prisma db push
	@cd apps/comments-service && COMMENTS_DATABASE_URL="file:./dev.db" npx prisma migrate deploy

# First-time local setup: containers + schema
dev-setup: dev-db dev-migrate
	@echo "Local DB ready. Run: make dev  (open https://localhost:8443)"

dev-proxy: .env certs
	$(DEV_PROXY) up -d

dev: .env keys certs dev-proxy
	@set -a && . ./.env && set +a; \
	port=$${APP_HTTPS_PORT:-8443}; \
	echo "Dev app (HTTPS): $${WEBAPP_URL:-https://localhost:$$port}"; \
	echo "  (self-signed cert — accept browser warning once; do not run make up at the same time)"; \
	set -a && . ./.env && set +a && npm run dev
