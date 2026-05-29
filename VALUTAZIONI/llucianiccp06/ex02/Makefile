NAME = identify

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g -std=c++98
INCLUDE = -I include
RM = rm -f

SRC = src/Base.cpp src/main.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re all fclean clean
