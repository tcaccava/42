# **************************************************************************** #
#                                 MINITALK                                     #
# **************************************************************************** #

NAME_BASE	= server client
NAME_BONUS	= server_bonus client_bonus

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

# Libft
LIBFT_DIR	= libft
LIBFT_A		= $(LIBFT_DIR)/libft.a
LIBFT_INC	= -I$(LIBFT_DIR)
LIBFT_LIB	= -L$(LIBFT_DIR) -lft

# Mandatory
SRCS_CLIENT	= srcs/client.c
SRCS_SERVER	= srcs/server.c

OBJS_CLIENT	= $(SRCS_CLIENT:.c=.o)
OBJS_SERVER	= $(SRCS_SERVER:.c=.o)

OBJS_ALL = $(OBJS_CLIENT) $(OBJS_SERVER)

.SILENT:

all: $(LIBFT_A) $(NAME_BASE)

$(LIBFT_A):
	@$(MAKE) --no-print-directory -s -C $(LIBFT_DIR)

client: $(OBJS_CLIENT)
	@echo "\033[32m[client] compiled!"
	@$(CC) $(CFLAGS) -o client $(OBJS_CLIENT) $(LIBFT_LIB)

server: $(OBJS_SERVER)
	@echo "\033[32m[server] compiled!"
	@$(CC) $(CFLAGS) -o server $(OBJS_SERVER) $(LIBFT_LIB)

clean:
	@rm -f $(OBJS_ALL)
	@$(MAKE) --no-print-directory -s -C $(LIBFT_DIR) clean

fclean: clean
	@echo "\033[32m[All] clean!"
	@rm -f client server client_bonus server_bonus
	@$(MAKE) --no-print-directory -s -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus
