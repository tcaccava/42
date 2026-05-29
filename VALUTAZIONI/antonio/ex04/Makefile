NAME		= Sed_is_for_losers
CXX			= c++
CXX_FLAGS	= -Wall -Wextra -Werror

CXX_FILES	= main.cpp input.cpp replaceStrings.cpp
OBJS		= $(CXX_FILES:.cpp=.o)

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
