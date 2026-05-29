NAME = RPN

SRCS = RPN.cpp \
	   main.cpp \

INCS = RPN.hpp


OBJS = $(SRCS:.cpp=.o)

CXX = c++

CLANGXX = clang++

FLAGS = -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME):
	@echo "Compiling..."
	@$(CXX) $(FLAGS) $(SRCS) -o $(NAME)
# 	@$(CLANGXX) $(FLAGS) $(SRCS) -o $(NAME)
	@echo "Done!"

clean:
	@echo "Cleaning up..."
	@rm -f $(OBJS)
	@echo "Done!"

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
