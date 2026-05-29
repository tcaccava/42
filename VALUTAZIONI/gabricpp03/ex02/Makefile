# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vhacman <vhacman@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/20 15:40:00 by vhacman           #+#    #+#              #
#    Updated: 2025/10/20 15:40:00 by vhacman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = FragTrap
CXX                     = c++
CXXFLAGS        = -Wall -Wextra -Werror -std=c++98 -Iincludes

SRC                     = src/main.cpp \
						  src/ClapTrap.cpp \
						  src/ScavTrap.cpp \
						  src/FragTrap.cpp

OBJ_DIR         = obj
OBJS            = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

ART_DIR         = ART/
ART1            = $(ART_DIR)art1
ART2            = $(ART_DIR)art2
ART3            = $(ART_DIR)art3

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@
	@[ -f "$(ART1)" ] && cat "$(ART1)" || true
	@echo ""

$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
		@rm -f $(OBJS)
		@[ -f "$(ART3)" ] && cat "$(ART3)" || true
		@echo ""

fclean: clean
		@rm -f $(NAME)
		@rm -rf $(OBJ_DIR)
		@[ -f "$(ART2)" ] && cat "$(ART2)" || true
		@echo ""

re: fclean all

.PHONY: all clean fclean re
