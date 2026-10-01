NAME = push_swap

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I.

SRC = main.c main_utils.c $(wildcard benchmark/*.c) $(wildcard parser/*.c) $(wildcard stack/*.c) $(wildcard algorithms/*.c) $(wildcard operations/*.c) $(wildcard flag/*.c)
OBJ = $(SRC:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compilato: $<"

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Adesso compilo libft"
	@make -C libft --no-print-directory
	@$(CC) $(CFLAGS) $(OBJ) -Llibft -lft -o $(NAME)
	@echo "push_swap creato!"

clean:
	@rm -f $(OBJ)

fclean: clean
	@make -C libft fclean --no-print-directory
	@rm -f $(NAME)
	@echo "$(NAME) eliminato!"

re: fclean all

.PHONY: all clean fclean re