NAME = program
CPP = c++ 
CFLAGS = -Werror -Wextra -Wall -std=c++98

SRC_DIR = src/
INC_DIR = include/
OBJ_DIR = obj/

SRC_FILES = main.cpp Animal.cpp Dog.cpp Cat.cpp WrongAnimal.cpp WrongCat.cpp

OBJ_FILES = $(SRC_FILES:.cpp=.o)
OBJ_DIR_FILES = $(addprefix $(OBJ_DIR),$(OBJ_FILES))

$(NAME): $(OBJ_DIR_FILES)
	$(CPP) $(CFLAGS) $(OBJ_DIR_FILES) -o $(NAME)

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	mkdir -p $(OBJ_DIR)
	$(CPP) $(CFLAGS)  -I $(INC_DIR) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(NAME)

fclean: clean
	rm -rf $(OBJ_DIR)

re: fclean all