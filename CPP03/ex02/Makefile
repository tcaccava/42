NAME = fragtrap
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf
SRCS = main.cpp ClapTrap.cpp ScavTrap.cpp FragTrap.cpp
OBJS = $(SRCS:.cpp=.o)
all : $(NAME)
$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 
clean :
	$(RM) $(OBJS)
fclean : clean
	$(RM) $(NAME)
re:fclean all