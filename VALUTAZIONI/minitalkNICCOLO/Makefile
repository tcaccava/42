NAME = minitalk

SRC_DIR = src
INCLUDE_DIR = includes
LIBFT_DIR = libft
PRINTF_DIR = printf

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCLUDE_DIR)

SRC = $(SRC_DIR)/client.c $(SRC_DIR)/server.c
SRC_BONUS = $(SRC_DIR)/client_bonus.c $(SRC_DIR)/server_bonus.c

OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

LIBFT = $(LIBFT_DIR)/libft.a
PRINTF = $(PRINTF_DIR)/libftprintf.a

all: $(LIBFT) $(PRINTF) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(PRINTF):
	@make -C $(PRINTF_DIR)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o client $(SRC_DIR)/client.o $(LIBFT) $(PRINTF)
	$(CC) $(CFLAGS) -o server $(SRC_DIR)/server.o $(LIBFT) $(PRINTF)

bonus: $(LIBFT) $(PRINTF) $(OBJ_BONUS)
	$(CC) $(CFLAGS) -o client_bonus $(SRC_DIR)/client_bonus.o $(LIBFT) $(PRINTF)
	$(CC) $(CFLAGS) -o server_bonus $(SRC_DIR)/server_bonus.o $(LIBFT) $(PRINTF)

clean:
	@rm -f $(OBJ) $(OBJ_BONUS)
	@make clean  -C $(LIBFT_DIR)
	@make clean  -C $(PRINTF_DIR)

fclean: clean
	@rm -f client server client_bonus server_bonus
	@make fclean -C $(LIBFT_DIR)
	@make fclean -C $(PRINTF_DIR)

re: fclean all

.PHONY: all clean fclean re bonus
