# =============================================================================
# Directories and Files
# =============================================================================
PROJECT_NAME	= transcendence
COMPOSE_FILE	= docker-compose.yml
COMPOSE 		= docker compose -p $(PROJECT_NAME) -f $(COMPOSE_FILE)

# =============================================================================
# Rules
# =============================================================================
up-build:
				@$(COMPOSE) up --build

up:
				@$(COMPOSE) up

down:
				@$(COMPOSE) down

clean-db:
				@$(COMPOSE) -p $(PROJECT_NAME) -f $(COMPOSE_FILE) down --volumes

fclean:
				@$(COMPOSE) -p $(PROJECT_NAME) -f $(COMPOSE_FILE) down --volumes --rmi all

help:
				@echo "usage: make [target]"
				@echo "  up            Start the containers"
				@echo "  up-build      Build and start the containers"
				@echo "  down          Stop the containers"
				@echo "  clean-db      Stop the containers and remove volumes"
				@echo "  fclean        Stop the containers, remove volumes and images"
				@echo "  re            Clean and rebuild everything"

re:				fclean up-build

# =============================================================================
# Phony Targets
# =============================================================================
.PHONY:				up-build up down clean-db fclean re
