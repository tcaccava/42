# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/10 14:46:56 by mde-matt          #+#    #+#              #
#    Updated: 2026/07/17 18:00:00 by mde-matt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= push_swap
BONUS		= checker

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

SRCS		= add_operators.c \
			  benchmark.c \
			  complexsolver.c \
			  ft_parser.c \
			  last_operators.c \
			  main.c \
			  mediumsolver.c \
			  mediumsolver_add_utils.c \
			  mediumsolver_utils.c \
			  operators.c \
			  simplesolver.c \
			  adaptivesolver.c

SRCS_BONUS	= checker_bonus.c \
			  ft_parser_bonus.c \
			  operators_bonus.c \
			  add_operators_bonus.c \
			  last_operators_bonus.c \
			  get_next_line/get_next_line.c \
			  get_next_line/get_next_line_utils.c

OBJS		= $(SRCS:.c=.o)
OBJS_BONUS	= $(SRCS_BONUS:.c=.o)

LIBFT		= libft/libft.a
PRINTF		= printf/libftprintf.a

all: $(LIBFT) $(PRINTF) $(NAME)

$(LIBFT):
	make -C libft

$(PRINTF): $(LIBFT)
	make -C printf

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(PRINTF) $(LIBFT) -o $(NAME)

bonus: $(LIBFT) $(PRINTF) $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(PRINTF) $(LIBFT) -o $(BONUS)

%.o: %.c
	$(CC) $(CFLAGS) -I. -I./get_next_line -c $< -o $@

clean:
	make -C libft clean
	make -C printf clean
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	make -C libft fclean
	make -C printf fclean
	rm -f $(NAME) $(BONUS)

re: fclean all

.PHONY: all clean fclean re bonus
