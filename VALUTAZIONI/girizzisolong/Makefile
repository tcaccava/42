# **************************************************************************** #
#                                  SO_LONG                                     #
# **************************************************************************** #

NAME = so_long
NAME_BONUS = so_long_bonus

# Directories
SRCS_DIR = src/
SRCS_BONUS_DIR = src_bonus/
OBJS_DIR = obj/
OBJS_BONUS_DIR = obj_bonus/
LIBFT_DIR = libft
MLX_DIR = libmlx

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a
MLX = $(MLX_DIR)/libmlx.a

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -fPIC
MLXFLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm
INCLUDES = -I./include -I$(LIBFT_DIR) -I$(MLX_DIR)

# Source files
SRCS = $(wildcard $(SRCS_DIR)*.c)
SRCS_BONUS = $(wildcard $(SRCS_BONUS_DIR)*.c)

# Object files
OBJS = $(notdir $(SRCS:.c=.o))
OBJS_PREFIXED = $(addprefix $(OBJS_DIR), $(OBJS))
OBJS_BONUS = $(notdir $(SRCS_BONUS:.c=.o))
OBJS_BONUS_PREFIXED = $(addprefix $(OBJS_BONUS_DIR), $(OBJS_BONUS))

# Colors for output
RED = \033[0;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
NC = \033[0m # No Color

# Rules
.PHONY: all clean fclean re libft mlx bonus help

all: $(OBJS_DIR) $(LIBFT) $(MLX) $(NAME)

bonus: $(OBJS_BONUS_DIR) $(LIBFT) $(MLX) $(NAME_BONUS)

MAKEFLAGS += --silent

# Create obj directories
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_BONUS_DIR):
	@mkdir -p $(OBJS_BONUS_DIR)

# Build libft
$(LIBFT):
	@$(MAKE) -sC $(LIBFT_DIR) > /dev/null 2>&1
	@echo -n "$(YELLOW)Building libft:              $(GREEN)["
	@for i in $$(seq 1 44); do \
		echo -n "#"; \
		sleep 0.02; \
	done; \
	echo "][OK]$(NC)"

# Build minilibx
$(MLX):
	@make -s -C $(MLX_DIR) > /dev/null 2>&1
	@echo -n "$(YELLOW)Building minilibx:           $(GREEN)["
	@for i in $$(seq 1 44); do \
		echo -n "#"; \
		sleep 0.02; \
	done; \
	echo "][OK]$(NC)"

# Main executable
$(NAME): $(OBJS_PREFIXED) $(LIBFT) $(MLX)
	@$(CC) -no-pie $(CFLAGS) $(OBJS_PREFIXED) $(LIBFT) $(MLX) -o $(NAME) $(MLXFLAGS)
	@bash -c '\
	echo -e "$(GREEN)..............................................................................."; sleep 0.1; \
	echo -e "$(GREEN)                                                                               "; sleep 0.1; \
	echo -e "To start the game:"; sleep 0.1; \
	echo -e "Run: $(NC)./$(NAME) mappina.ber"; sleep 0.1; \
	'
	@echo "\n"

# Bonus executable
$(NAME_BONUS): $(OBJS_BONUS_PREFIXED) $(LIBFT) $(MLX)
	@$(CC) -no-pie $(CFLAGS) $(OBJS_BONUS_PREFIXED) $(LIBFT) $(MLX) -o $(NAME_BONUS) $(MLXFLAGS)
	@bash -c '\
	echo -e "$(GREEN)                                                                               "; sleep 0.1; \
	echo -e "$(GREEN)                                                                  (O) /bonus/  "; sleep 0.1; \
	echo -e "$(GREEN)                                                                  /|\/         "; sleep 0.1; \
	echo -e "$(GREEN)                                                                 / |           "; sleep 0.1; \
	echo -e "$(GREEN)                                                                  / \          "; sleep 0.1; \
	echo -e "$(GREEN)................................................................./...\........."; sleep 0.1; \
	echo -e "$(GREEN)                                                                               "; sleep 0.1; \
	echo -e "To start the game:"; sleep 0.1; \
	echo -e "Run: $(NC)./$(NAME_BONUS) mappona.ber"; sleep 0.1; \
	'
	@echo "\n"

# Object files compilation
$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@printf "$(YELLOW)Compiling                    $(GREEN)[OK]\r" $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Object files compilation for bonus
$(OBJS_BONUS_DIR)%.o: $(SRCS_BONUS_DIR)%.c
	@printf "$(YELLOW)Compiling                    $(GREEN)[OK]\r" $@
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean object files
clean:
	@echo -n "$(BLUE)Cleaning objects:            $(GREEN)["
	@for i in $$(seq 1 41); do \
		echo -n "#"; \
		sleep 0.01; \
	done; \
	echo "][CLEAN]$(NC)"
	@rm -rf $(OBJS_DIR) $(OBJS_BONUS_DIR)
	@make clean -C $(LIBFT_DIR) > /dev/null 2>&1
	@make clean -C $(MLX_DIR) > /dev/null 2>&1

# Clean everything
fclean: clean
	@echo -n "$(BLUE)Cleaning all:                $(GREEN)["
	@for i in $$(seq 1 41); do \
		echo -n "#"; \
		sleep 0.01; \
	done; \
	echo "][CLEAN]$(NC)"
	@rm -f $(NAME) $(NAME_BONUS)
	@make fclean -C $(LIBFT_DIR) > /dev/null 2>&1

# Rebuild everything
re: fclean all

# Help
help:
	@echo "$(BLUE)Available targets:$(NC)"
	@echo "  $(GREEN)all$(NC)      - Build the project"
	@echo "  $(GREEN)bonus$(NC)    - Build the bonus version"
	@echo "  $(GREEN)clean$(NC)    - Remove object files"
	@echo "  $(GREEN)fclean$(NC)   - Remove object files and executables"
	@echo "  $(GREEN)re$(NC)       - Rebuild everything"
	@echo "  $(GREEN)help$(NC)     - Show this help"