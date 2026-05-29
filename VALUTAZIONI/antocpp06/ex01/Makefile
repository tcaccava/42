NAME		= Serializer
CXX			= c++
CXX_FLAGS	= -Wall -Werror -Wextra -std=c++98

SRCS		= main.cpp Serializer.cpp
HEADERS		= ScalarConverter.hpp Data.hpp
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

