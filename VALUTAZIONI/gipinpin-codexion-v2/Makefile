NAME = codexion

CC = cc
FLAGS = -Wall -Werror -Wextra -pthread
RM = rm -f

FILES = main.c parsing.c error.c coder.c coder_moves.c dongle.c sim.c utils.c log.c time_utils.c pq.c pq_utils.c monitor.c
OBJS = $(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
