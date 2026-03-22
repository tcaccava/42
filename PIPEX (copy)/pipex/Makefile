NAME = pipex
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c pipex.c ft_split.c utils.c path.c get_next_line.c get_next_line_utils.c bonus.c ft_printf.c
OBJS = $(SRCS:.c=.o)
all: $(NAME)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)
re: fclean all