NAME = btc

SRCS = BitcoinExchange.cpp \
	main.cpp \
	utils.cpp \

FLAGS = c++ -Wall -Wextra -Werror -std=c++98

FLAGS_O = clang++ -Wall -Wextra -Werror -std=c++98

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME):
	@echo "Compiling..."
	@$(FLAGS) $(SRCS) -o $(NAME)
# 	@$(FLAGS_O) $(SRCS) -o $(NAME)
	@echo "READY."

clean:
	@rm -rf *.o
	@echo "Object deleted"

fclean: clean
	@rm -rf $(NAME)
	@echo "All cleared"

re: fclean all

.PHONY: all clean fclean re
