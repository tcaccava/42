# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/30 14:55:22 by mla-mona          #+#    #+#              #
#    Updated: 2024/08/30 14:55:58 by mla-mona         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fixed

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