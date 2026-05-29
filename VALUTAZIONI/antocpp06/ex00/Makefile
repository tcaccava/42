NAME		= ScalarConverter
CXX			= c++
CXX_FLAGS	= -Wall -Werror -Wextra -std=c++98

SRCS		= ScalarConverter.cpp main.cpp
HEADERS		= ScalarConverter.hpp
OBJS		= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXX_FLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

