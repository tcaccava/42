# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rauer <rauer@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/26 16:48:00 by racherom          #+#    #+#              #
#    Updated: 2025/03/14 17:14:47 by rauer            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test
SRC = main.cpp AMateria.cpp Character.cpp Cure.cpp Ice.cpp MateriaSource.cpp
OBJ = $(SRC:.cpp=.o)
CXX = c++ -Wall -Wextra -Werror -std=c++98

$(NAME): $(OBJ)
	$(CXX) $(OBJ) $(DEBUG) -o $@

all: $(NAME)

%.o: %.cpp
	$(CXX) $(DEBUG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.phony: all clean fclean re
