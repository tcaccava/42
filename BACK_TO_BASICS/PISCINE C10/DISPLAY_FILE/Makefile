NAME = ft_display_file
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = display_file.c
OBJS = $(SRCS:.c=.o)
RM = rm -f
all : $(NAME)
$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean :
	$(RM) $(OBJS)
fclean : clean
	$(RM) $(NAME)
re: fclean all