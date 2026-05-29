NAME = push_swap

LIBFT = libft/libft.a

SRCS = little_sort.c arg_check2.c index_sort.c utils.c utils2.c push_swap.c mem_utils.c hybrid_sorting.c stack_operation.c arg_check.c stack_op_reverse.c stack_op_rotate.c

OBJS = $(SRCS:.c=.o)

CC = gcc

CFLAGS = -Wall -Wextra -Werror 

RM = rm -f

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT)

$(LIBFT):
	$(MAKE) -C libft

clean:
	$(RM) $(OBJS)
	$(MAKE) -C libft clean

fclean: clean
	@echo "Let's clean! :D"
	$(RM) $(NAME) 
	$(MAKE) -C libft clean

re: fclean all

.PHONY: all clean fclean re
