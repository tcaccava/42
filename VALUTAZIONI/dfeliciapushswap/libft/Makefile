# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/25 14:45:46 by mde-matt          #+#    #+#              #
#    Updated: 2026/05/25 15:10:35 by mde-matt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = ft_atoi.c ft_isdigit.c ft_lstdelone.c ft_memchr.c \
       ft_putendl_fd.c ft_striteri.c ft_strncmp.c ft_toupper.c \
       ft_bzero.c ft_isprint.c ft_lstiter.c ft_memcmp.c \
       ft_putnbr_fd.c ft_strjoin.c ft_strnstr.c ft_calloc.c \
       ft_itoa.c ft_lstlast.c ft_memcpy.c ft_putstr_fd.c \
       ft_strlcat.c ft_strrchr.c ft_isalnum.c ft_lstadd_back.c \
       ft_lstmap.c ft_memmove.c ft_split.c ft_strlcpy.c \
       ft_strtrim.c ft_isalpha.c ft_lstadd_front.c ft_lstnew.c \
       ft_memset.c ft_strchr.c ft_strlen.c ft_substr.c \
       ft_isascii.c ft_lstclear.c ft_lstsize.c ft_putchar_fd.c \
       ft_strdup.c ft_strmapi.c ft_tolower.c
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
