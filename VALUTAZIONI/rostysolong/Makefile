# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: [your_name] <[your_email]>                 +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:00:00 by [your_name]      #+#    #+#              #
#    Updated: 2025/04/29 12:00:00 by [your_name]     ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = so_long

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

# Source files
SRC_DIR = src/
SRC_FILES = main.c \
			init.c \
 			parse_map.c \
			parse_map_utils.c \
			validate_map.c \
			check_path.c \
			render.c \
			handle_input.c \
			player_movement.c \
			game_logic.c \
			cleanup.c \
			validate_map_utils.c \
			render_utils.c \
			check_path_utils.c \
			check_path_items.c \
			map_utils.c 

SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))

# Object files
OBJ_DIR = obj/
OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

# Libraries
LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a

MLX_DIR = mlx/
MLX = $(MLX_DIR)libmlx.a

# Include directory
INC_DIR = includes/
INC = -I$(INC_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR)

# Library flags
LIB_FLAGS = -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

all: $(NAME)

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Compile libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Compile MLX
$(MLX):
	$(MAKE) -C $(MLX_DIR)

# Compile object files
$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Link executable
$(NAME): $(LIBFT) $(MLX) $(OBJS)
	$(CC) $(OBJS) $(LIB_FLAGS) -o $(NAME)
	@echo "$(NAME) created!"

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MLX_DIR) clean
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
