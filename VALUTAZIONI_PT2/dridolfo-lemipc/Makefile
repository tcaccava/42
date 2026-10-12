NAME		= lemipc

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -Iincludes -I$(LIBFT_DIR)

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

SRCS_DIR	= srcs
OBJS_DIR	= objs
SRCS		= main.c \
			  ipc.c \
			  board.c \
			  comms.c \
			  ai.c \
			  player.c \
			  viewer.c \
			  utils.c
OBJS		= $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))
HEADER		= includes/lemipc.h

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Always delegate to libft's own Makefile; we only relink if libft.a changed
$(LIBFT): $(LIBFT_DIR)/Makefile FORCE
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(LIBFT_DIR)/Makefile:
	@echo "libft is missing: run 'git submodule update --init'" && false

clean:
	rm -rf $(OBJS_DIR)
	@if [ -f $(LIBFT_DIR)/Makefile ]; then \
		$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean; fi

fclean: clean
	rm -f $(NAME)
	@if [ -f $(LIBFT_DIR)/Makefile ]; then \
		$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean; fi

re: fclean all

FORCE:

.PHONY: all clean fclean re FORCE
