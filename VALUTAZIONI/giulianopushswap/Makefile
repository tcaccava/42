CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = push_swap

SRCS = push_swap.c utils_nbr.c utils_str.c parser.c moves.c double_moves.c lst_utils.c check_index.c struct_intializers.c simple_min_max_extraction_method.c chunk_sort.c radix_sort.c parsing_size.c bench.c greatest_or_smallest.c order.c
OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

%o: %c
	$(CC) $(CFLAGS) $< -c $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re
