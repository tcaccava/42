NAME = PmergeMe

TESTER_NAME = Tester

SRCS = PmergeMe.cpp \
		listFunctions.cpp \
		vectorFunctions.cpp \

SRC_TESTER = tester.cpp

SRC_PROGRAM = main.cpp

INCS = PmergeMe.hpp

OBJS = $(SRCS:.cpp=.o) $(SRC_PROGRAM:.cpp=.o) $(SRC_TESTER:.cpp=.o)

CXX = c++
CLANGXX = clang++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g

all: $(NAME)

$(NAME):
	@echo "Compiling..."
# 	@$(CLANGXX) $(FLAGS) $(SRC_PROGRAM) $(SRCS) -o $(NAME)
	@$(CXX) $(FLAGS) $(SRC_PROGRAM) $(SRCS) -o $(NAME)
	@echo "Done!"

clean:
	@echo "Cleaning up..."
	@rm -f $(OBJS)
	@echo "Done!"

fclean: clean
	@rm -f $(NAME)
	@rm -f $(TESTER_NAME)
	@echo "Removed executable files."

re: fclean all

tester: re
	@echo "Compiling the tester..."
	@$(CXX) $(FLAGS) $(SRC_TESTER) $(SRCS) -o $(TESTER_NAME)

.PHONY: all clean fclean re tester
