NAME = a.out
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf
SRCS = main.cpp Fixed.cpp Point.cpp bsp.cpp
OBJS = $(SRCS:.cpp=.o)
all : $(NAME)
$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean: 
	$(RM) $(OBJS)
fclean: clean
	$(RM) $(NAME)
re : fclean all