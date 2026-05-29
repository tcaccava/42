NAME = so_long

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft
MLX_DIR = mlx

SRCS = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/errors.c \
	$(SRC_DIR)/map_read.c \
	$(SRC_DIR)/map_parse.c \
	$(SRC_DIR)/map_parse_2.c \
	$(SRC_DIR)/map_check.c \
	$(SRC_DIR)/map_player.c \
	$(SRC_DIR)/mlx_setup.c \
	$(SRC_DIR)/textures.c \
	$(SRC_DIR)/render.c \
	$(SRC_DIR)/hooks.c \
	$(SRC_DIR)/moves.c

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

LIBFT = $(LIBFT_DIR)/libft.a
MLX = $(MLX_DIR)/libmlx.a
MLX_LIBS = -L$(MLX_DIR) -lmlx -lX11 -lXext -lm

all: $(LIBFT) $(MLX) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(MLX):
	@make -C $(MLX_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX_LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Iincludes -I$(LIBFT_DIR) -I$(MLX_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@make -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
