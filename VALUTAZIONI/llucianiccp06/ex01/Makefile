NAME = serializer

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g
INCLUDE = -I include

RM = rm -f

SRC = src/main.cpp src/Serialize.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(NAME) $(OBJ)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re clean fclean all
