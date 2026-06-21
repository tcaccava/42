# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/30 16:56:19 by ilnassi           #+#    #+#              #
#    Updated: 2026/05/30 16:56:21 by ilnassi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = zombie
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98

INC_DIR     = includes
SRC_DIR     = src

SRCS        = main.cpp \
              $(SRC_DIR)/Zombie.cpp \
              $(SRC_DIR)/newZombie.cpp \
              $(SRC_DIR)/randomChump.cpp

OBJS        = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
