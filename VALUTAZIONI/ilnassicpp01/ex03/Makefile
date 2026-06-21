# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/08 16:42:43 by ilnassi           #+#    #+#              #
#    Updated: 2026/06/08 16:42:45 by ilnassi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        =   unnecessaryViolence

CC          =   c++
FLAGS       =   -Wall -Wextra -Werror -std=c++98 -I./includes

# Cartelle
INC_DIR     =   includes
SRC_DIR     =   src

SRCS        =   main.cpp \
		src/Weapon.cpp \
                src/HumanA.cpp \
                src/HumanB.cpp

OBJS        =   $(SRCS:.cpp=.o)

RM          =   rm -f

all:            $(NAME)

$(NAME):        $(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o:            %.cpp
				$(CC) $(FLAGS) -c $< -o $@

clean:
				$(RM) $(OBJS)

fclean:         clean
				$(RM) $(NAME)

re:             fclean all

.PHONY:         all clean fclean re
