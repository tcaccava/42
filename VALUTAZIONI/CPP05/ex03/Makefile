NAME = a.out

SRCS = $(wildcard *.cpp)

FLAGS = -Wall -Wextra -Werror -Wshadow -Wno-shadow -std=c++98

OBJS	= $(SRCS:.cpp=.o)

CPP		= g++

%.o: %.cpp
			$(CPP) $(FLAGS) -c $< -o $@


all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(OBJS) $(SRC) -o $(NAME) $(FLAGS)

clean:
	rm -rf $(NAME) $(OBJS) tree_shrubbery

fclean: clean

re: fclean all

.PHONY: all clean fclean re