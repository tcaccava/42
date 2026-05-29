# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: valucare <valucare@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/29 14:08:57 by valucare          #+#    #+#              #
#    Updated: 2026/03/07 14:08:59 by valucare         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = fractol

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -Imlx

MLX_FLAGS = -Lmlx -lmlx -L/usr/lib -Imlx -lX11 -lXext -lm

MLX_DIR     = mlx

SRCS        = main.c init.c parse.c hooks.c render.c \
              complex.c color.c utils.c
OBJS        = $(SRCS:.c=.o)

MLX         = $(MLX_DIR)/libmlx.a

all: $(NAME)

$(NAME): $(MLX) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

$(MLX):
	@make -C $(MLX_DIR)

clean:
	rm -f $(OBJS)
	@make -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
