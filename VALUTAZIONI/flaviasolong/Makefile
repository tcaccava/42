NAME := so_long
CC := cc
CFLAGS := -Wall -Wextra -Werror
OBJDIR := obj

SRCS := src/main.c src/map.c src/validate.c src/path_check.c src/game.c src/render.c src/input.c src/error.c

LIBFT_SRCS :=  lib/libft/ft_strlen.c lib/libft/ft_strdup.c lib/libft/ft_strjoin.c lib/libft/ft_strchr.c lib/libft/ft_strncmp.c lib/libft/ft_calloc.c lib/libft/ft_bzero.c lib/libft/ft_memset.c lib/libft/ft_memcpy.c lib/libft/ft_split.c lib/libft/ft_putstr_fd.c lib/libft/ft_putendl_fd.c lib/libft/ft_putnbr_fd.c

PRINTF_SRCS := lib/ft_printf/ft_printf.c

GNL_SRCS := lib/get_next_line/get_next_line.c

OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
LIBFT_OBJS := $(addprefix $(OBJDIR)/,$(LIBFT_SRCS:.c=.o))
PRINTF_OBJS := $(addprefix $(OBJDIR)/,$(PRINTF_SRCS:.c=.o))
GNL_OBJS := $(addprefix $(OBJDIR)/,$(GNL_SRCS:.c=.o))

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    MLX_DIR ?= minilibx-linux
    MLX_LIB := $(MLX_DIR)/libmlx.a
    MLX_FLAGS := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz
else
    MLX_DIR ?= minilibx_macos
    MLX_LIB := $(MLX_DIR)/libmlx.a
    MLX_FLAGS := -L$(MLX_DIR) -lmlx -framework OpenGL -framework AppKit
endif

INCLUDES := -Iinclude -Ilib/libft -Ilib/ft_printf -Ilib/get_next_line -I$(MLX_DIR)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_OBJS) $(PRINTF_OBJS) $(GNL_OBJS) $(MLX_LIB)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_OBJS) $(PRINTF_OBJS) $(GNL_OBJS) $(MLX_FLAGS) -o $(NAME)

$(MLX_LIB):
	@if [ -d "$(MLX_DIR)" ]; then 		$(MAKE) -C $(MLX_DIR); 	else 	git clone https://github.com/42Paris/minilibx-linux minilibx-linux; 		exit 1; 	fi

minilibx:
	git clone https://github.com/42Paris/minilibx-linux minilibx-linux

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	@if [ -d "$(MLX_DIR)" ]; then $(MAKE) -C $(MLX_DIR) clean; fi

fclean: clean
	rm -f $(NAME)

re: fclean all

norminette:
	norminette $(SRCS) include/ lib/libft/ lib/ft_printf/ lib/get_next_line/

.PHONY: all clean fclean re
