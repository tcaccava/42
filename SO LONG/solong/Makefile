NAME = so_long
CC = gcc
CFLAGS = -Wall -Wextra -Werror
MLX_DIR = ./minilibx-linux
MLX = -L$(MLX_DIR) -lmlx -lX11 -lXext -lm
SRCS = main.c get_next_line.c so_long_utils.c bonus.c gnl_utils.c ft_printf.c ft_printf_utils.c collectibles.c check.c player.c input.c exit.c map.c valid_path.c enemies.c animations.c ft_itoa.c
OBJ = $(SRCS:.c=.o)

all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(MLX)
%.o: %.c
	$(CC) $(CFLAGS) -I$(MLX_DIR) -c $< -o $@
bonus: all
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean
	make all
.PHONY: all clean fclean re
