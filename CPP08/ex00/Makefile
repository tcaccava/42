NAME = containers
CFLAGS = -Wall -Wextra -Werror -std=c++98
CC = c++
SRCS = main.cpp
RM = rm -rf
OBJS = $(SRCS:.cpp=.o)

all : $(NAME)
$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean :
	$(RM) $(OBJS)
fclean : clean
	$(RM) $(NAME)
re : fclean all

