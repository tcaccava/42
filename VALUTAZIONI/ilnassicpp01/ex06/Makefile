# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/08 16:16:52 by ilnassi           #+#    #+#              #
#    Updated: 2026/06/08 16:17:15 by ilnassi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = harlFilter

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -I includes

SRCS = 		main.cpp \
			src/Harl.cpp
			
OBJS        = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
