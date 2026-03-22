NAME = reference
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=++c98
RM = rm -rf
SRCS = Weapon.cpp HumanA.cpp HumanB.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
all : $(NAME)
$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean :
	$(RM) $(OBJS)
fclean : clean
	$(RM) $(NAME)
re : fclean all