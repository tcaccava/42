NAME = phonebook
CC = c++
CCFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -rf
SRCS = PhoneBook.cpp Contact.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
all : $(NAME)
$(NAME) : $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $(NAME)
clean : 
	$(RM) $(OBJS)
fclean : clean
	$(RM) $(NAME)
re: fclean all