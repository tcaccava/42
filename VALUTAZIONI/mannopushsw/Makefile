# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmanno <fmanno@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/23 20:25:18 by fmanno            #+#    #+#              #
#    Updated: 2025/08/25 17:25:12 by fmanno           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = push_swap

CC = cc
CCFLAGS = -Wall -Werror -Wextra

LIBFT = libft/libft.a
FT_PRINTF = libft/ft_printf/libftprintf.a
LIBS = $(LIBFT) $(FT_PRINTF)

OP_DIR = operations/
SORT_DIR = sort/
UTILS_DIR = utils/

SRCS = $(wildcard $(OP_DIR)/*.c) $(wildcard $(SORT_DIR)/*.c) \
       $(wildcard $(UTILS_DIR)/*.c) input_validation.c push_swap.c

OBJS = $(SRCS:.c=.o)

INCLUDES = -Iincludes -Ilibft -Ilibft/ft_printf -I$(SRCS_DIR)

all: ${NAME}

${NAME}: ${OBJS} ${LIBS}
	${CC} ${CCFLAGS} ${OBJS} ${LIBS} -o ${NAME}

$(LIBFT):
	$(MAKE) -C libft

$(FT_PRINTF):
	$(MAKE) -C libft/ft_printf

%.o: %.c
	${CC} ${CCFLAGS} -c $< -o $@

clean:
	rm -f ${OBJS}
	rm -f libft/*.o
	rm -f libft/ft_printf/*.o

fclean: clean
	rm -f ${NAME}
	rm -f libft/libft.a
	rm -f libft/ft_printf/libftprintf.a

re: fclean all

.PHONY: all clean fclean re
