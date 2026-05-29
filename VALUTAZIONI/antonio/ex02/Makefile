NAME		= HI_THIS_IS_BRAIN
CXX			= c++
CXX_FLAGS	= -Wall -Wextra -Werror

CXX_FILES	= main.cpp
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
