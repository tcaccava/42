# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 15:55:09 by mla-mona          #+#    #+#              #
#    Updated: 2024/09/03 16:30:32 by mla-mona         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = claptrap

SRC = $(wildcard *.cpp)

FLAGS = -Wall -Wextra -Werror -std=c++98

$(NAME) :
	c++ $(SRC) $(FLAGS) -o $(NAME)

all : $(NAME)

clean :
	rm -f $(NAME)

fclean : clean
	rm -f $(NAME)

re: fclean all