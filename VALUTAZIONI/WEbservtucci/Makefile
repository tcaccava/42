NAME = webserv

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

SRC = $(wildcard $(SRC_DIR)/*.cpp)

OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.cpp=.o)))

FLAGS := -Wall -Werror -Wextra -std=c++98 -I$(INC_DIR) -I$(SRC_DIR)

# COLORI
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
DEFAULT = \033[0m

all: $(NAME)

	@mkdir -p var/www/uploads
	@mkdir -p var/www/uploads/file
	@mkdir -p var/www/uploads/json
	@mkdir -p var/www/uploads/raw

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@c++ $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	@c++ $(FLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) created!$(DEFAULT)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW)Object files and directory deleted!$(DEFAULT)"
	@rm -rf var/www/uploads

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) deleted!$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean re
