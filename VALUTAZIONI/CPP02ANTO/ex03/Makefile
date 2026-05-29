NAME		= BSP

CXX			= c++
CXX_FLAGS	= -Wall -Wextra -Werror -std=c++98

CXX_FILES	= main.cpp Fixed.cpp Point.cpp bsp.cpp
OBJS		= $(CXX_FILES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXX_FLAGS) $^ -o $@

%.o:%.cpp $(HPP_DIR)/*
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
