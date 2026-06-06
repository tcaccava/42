NAME = inception

all:
	@mkdir -p /home/$(shell whoami)/data/mariadb
	@mkdir -p /home/$(shell whoami)/data/wordpress
	@docker compose -f srcs/docker-compose.yaml up -d --build

down:
	@docker compose -f srcs/docker-compose.yaml down

clean:
	@docker compose -f srcs/docker-compose.yaml down -v --rmi all

fclean: clean
	@sudo rm -rf /home/$(shell whoami)/data

re: fclean all

.PHONY: all down clean fclean re