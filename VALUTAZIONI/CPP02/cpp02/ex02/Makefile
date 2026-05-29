# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mla-mona <mla-mona@student.42roma.it>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/30 15:30:47 by mla-mona          #+#    #+#              #
#    Updated: 2024/08/30 16:02:16 by mla-mona         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fixed_point

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