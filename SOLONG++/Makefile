NAME = so_long
CC = gcc
MLX_DIR = ./minilibx-linux
MLX = -L$(MLX_DIR) -lmlx -lX11 -lXext -lm
SRCS = $(wildcard *.c)
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
