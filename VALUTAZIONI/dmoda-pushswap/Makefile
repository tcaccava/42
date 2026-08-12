NAME    = push_swap

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror

SRCS    = ft_bench.c ft_block.c ft_bubble.c ft_control.c \
          ft_disorder.c ft_main.c ft_operations.c ft_print.c \
          ft_push_swap_utils.c ft_quick.c ft_stack_utils.c

OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
