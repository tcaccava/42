NAME = iter

CXX = c++
CCXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
INCLUDE = -I include

RM = rm -f


SRC = src/main.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CCXXFLAGS) $(INCLUDE) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CCXXFLAGS) $(INCLUDE) -c $< -o $@


clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re all fclean clean

