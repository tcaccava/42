# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdecarli <mdecarli@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/01 16:53:46 by mdecarli          #+#    #+#              #
#    Updated: 2026/07/01 16:58:04 by mdecarli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = get_next_line.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -D BUFFER_SIZE=42

SRCS = get_next_line.c get_next_line_utils.c 
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re