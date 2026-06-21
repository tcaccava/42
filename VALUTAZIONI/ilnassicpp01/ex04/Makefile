# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/08 16:25:43 by ilnassi           #+#    #+#              #
#    Updated: 2026/06/08 16:25:46 by ilnassi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME     = replace

CXX      = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -I includes

SRCS = 		main.cpp \
			src/replaceLine.cpp

OBJS     = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
