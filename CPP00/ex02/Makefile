NAME = account
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
SRCS = Account.cpp tests.cpp
OBJS = $(SRCS:.cpp=.o)
RM = rm -rf
all : $(NAME)
$(NAME) : $(OBJS)
	 $(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean : 
	$(RM) $(OBJS)
fclean : clean
	$(RM) $(NAME)
re : fclean all