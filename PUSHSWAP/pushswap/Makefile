NAME = push_swap
BONUS = checker
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I.
SRCS = ft_atoi.c ft_printf.c input_check.c push_swap.c push.c reverse.c rotate.c sort_3_2_5.c ft_split.c stack_op.c swap.c main.c five+.c five+2.c five+3.c five+4.c get_next_line.c get_next_line_utils.c ft_strcmp.c
BONUS_SRC = bonus/bonus.c
OBJS = $(SRCS:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o) 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@
all :$(NAME)
bonus: $(BONUS)
$(BONUS): $(BONUS_OBJ) $(filter-out main.o, $(OBJS))
	$(CC) $(CFLAGS) -o $(BONUS) $(BONUS_OBJ) $(filter-out main.o, $(OBJS))
clean:
	rm -f $(OBJS) $(BONUS_OBJ)
fclean: clean
	rm -f $(NAME) $(BONUS)
re: fclean all