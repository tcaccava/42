NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g -fsanitize=thread 
CPPFLAGS = -Iinclude

RM = rm -f
RMDIR = rm -rf
MKDIR = mkdir -p

SRC_DIR = src
OBJ_DIR = obj

SRCS = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/parser.c \
	$(SRC_DIR)/coder.c \
	$(SRC_DIR)/monitor.c \
	$(SRC_DIR)/monitor_utils.c \
	$(SRC_DIR)/heap.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/utils2.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

HEADERS = include/codexion.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RMDIR) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re