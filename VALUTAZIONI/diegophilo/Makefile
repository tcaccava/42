NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

INCLUDES = -Iincludes

SRC_DIR = src
SRCS = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/parse.c \
	$(SRC_DIR)/init.c \
	$(SRC_DIR)/simulation.c \
	$(SRC_DIR)/routine.c \
	$(SRC_DIR)/routine_helpers_a.c \
	$(SRC_DIR)/routine_helpers_b.c \
	$(SRC_DIR)/monitor.c \
	$(SRC_DIR)/time_utils.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/cleanup.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
