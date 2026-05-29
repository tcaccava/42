NAME = ScalarConverter

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
INCLUDE = -I include
RM = rm -f

SRC = src/ScalarConverter.cpp src/main.cpp

OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CFLAGS) $(INCLUDE) -o $(NAME) $(OBJ)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re fclean clean all
