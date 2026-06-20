# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/16 15:43:26 by mpapale           #+#    #+#              #
#    Updated: 2026/06/16 15:43:26 by mpapale          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# If the subject requires it, you must submit a Makefile that compiles your source
# files to the required output with the flags -Wall, -Wextra, and -Werror, using cc.
# Additionally, your Makefile must not perform unnecessary relinking.
#
# Your Makefile must contain at least the rules $(NAME), all, clean, fclean and re.
# 
# To submit bonuses for your project, you must include a bonus rule in your Makefile,
# which will add all the various headers, libraries, or functions that are not allowed in
# the main part of the project. Bonuses must be placed in _bonus.{c/h} files, unless
# the subject specifies otherwise. The evaluation of mandatory and bonus parts is
# conducted separately.
#
# If your project allows you to use your libft, you must copy its sources and its
# associated Makefile into a libft folder. Your project’s Makefile must compile
# the library by using its Makefile, then compile the project

NAME = libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

AR = ar rcs
RM = rm -f

SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_strlcpy.c ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_strnstr.c \
	ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_memchr.c ft_memcmp.c \
	ft_toupper.c ft_tolower.c \
	ft_atoi.c ft_itoa.c \
	ft_calloc.c ft_strdup.c \
	ft_strmapi.c ft_striteri.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c \
	ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c \

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY : all clean fclean re bonus