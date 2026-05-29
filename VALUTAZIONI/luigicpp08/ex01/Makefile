NAME = program
CPP = c++
CPP_FLAGS = -Werror -Wextra -Wall -std=c++98

SRC_DIR = src/
INC_DIR = inc/
OBJ_DIR = obj/

SRCS_FILES = main.cpp Span.cpp

OBJ_FILES = $(SRCS_FILES:.cpp=.o)
OBJ_DIR_FILES = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

all: $(NAME)

$(NAME): $(OBJ_DIR_FILES)
	$(CPP) $(CPP_FLAGS) $(OBJ_DIR_FILES) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CPP_FLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	rm -f $(NAME)

fclean: clean
	rm -rf $(OBJ_DIR)

re: fclean all