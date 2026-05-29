# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apuddu <apuddu@student.42roma.it>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/03 18:13:07 by apuddu            #+#    #+#              #
#    Updated: 2025/03/03 00:56:38 by apuddu           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = PmergeMe

SOURCES  := $(wildcard *.cpp)
INCLUDES  := $(wildcard *.hpp) $(wildcard *.tpp)

OBJECTS = $(SOURCES:.cpp=.o)

CC = c++
CFLAGS =  -O3 -Wall -Wextra -Werror -g -std=c++98 #-fsanitize=address -fsanitize=undefined

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS) $(CFLAGS)

%.o: %.cpp $(INCLUDES)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
