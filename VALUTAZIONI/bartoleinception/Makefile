NAME		= inception
USER		= fbartole
DATA_PATH	= /home/$(USER)/data
COMPOSE		= docker compose -f srcs/docker-compose.yml --env-file srcs/.env

all: up

setup:
	@mkdir -p $(DATA_PATH)/db $(DATA_PATH)/wordpress
	@grep -q "$(USER).42.fr" /etc/hosts || echo "127.0.0.1 $(USER).42.fr" | sudo tee -a /etc/hosts

build: setup
	$(COMPOSE) build

up: setup
	$(COMPOSE) up -d --build

down:
	$(COMPOSE) down

stop:
	$(COMPOSE) stop

start:
	$(COMPOSE) start

logs:
	$(COMPOSE) logs -f

ps:
	$(COMPOSE) ps

clean: down
	-docker volume rm $$(docker volume ls -q | grep -E "$(NAME)|wordpress|mariadb") 2>/dev/null || true

fclean: clean
	-docker system prune -af --volumes
	-sudo rm -rf $(DATA_PATH)

re: fclean up

.PHONY: all setup build up down stop start logs ps clean fclean re
