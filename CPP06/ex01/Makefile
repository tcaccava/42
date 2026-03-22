NAME = serializer
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
SRCS = Data.cpp Serializer.cpp main.cpp
RM = rm -rf
OBJS = $(SRCS:.cpp=.o)
all: $(NAME)
$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean : 
	$(RM) $(OBJS)
fclean : clean
	$(RM) $(NAME)
re: fclean all