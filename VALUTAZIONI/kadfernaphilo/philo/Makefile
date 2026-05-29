# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kadferna <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/09 12:00:50 by kadferna          #+#    #+#              #
#    Updated: 2025/05/16 14:53:13 by kadferna         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
SRCS_DIR = ./srcs
CFLAGS = -Wall -Werror -Wextra -pthread 
SRCS = $(SRCS_DIR)/utils.c \
		$(SRCS_DIR)/utils1.c \
		$(SRCS_DIR)/utils2.c \
		$(SRCS_DIR)/utils3.c \
		$(SRCS_DIR)/frees.c  \
		$(SRCS_DIR)/errors.c \
		$(SRCS_DIR)/checks.c \
		$(SRCS_DIR)/parsing.c \
		main.c 
OBJS = ${SRCS:.c=.o}
NAME = philo

all : $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
