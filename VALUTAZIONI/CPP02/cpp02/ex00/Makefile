# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/28 18:50:38 by mla-mona          #+#    #+#              #
#    Updated: 2024/08/28 18:51:23 by mla-mona         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = orthodox

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