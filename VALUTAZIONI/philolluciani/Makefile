NAME = philo
CC = cc 
CFLAGS = -Wall -Wextra -Werror -g
SRCS = check_input.c main.c philosophers.c routine.c \
		utils.c init.c

all: $(NAME)

OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@printf "\n\033[1A\033[K"
	@printf "$(NAME) compiled!\n"

%.o: %.c
	@printf "\033; Generating $(NAME) objects... %-33.33s\r" $@
	$(CC) $(CFLAGS) -c $< -o $@	

clean:
		$(RM) $(OBJS)

fclean: clean
		$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus