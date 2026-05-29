NAME = philosophers
BONUS_NAME = philosophers_bonus
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

MANDATORY_DIR = philo
BONUS_DIR = philo_bonus
LIBFT_DIR = libft

SRCS =	$(MANDATORY_DIR)/main.c \
    	$(MANDATORY_DIR)/utils.c \
		$(MANDATORY_DIR)/utils2.c \
		$(MANDATORY_DIR)/init.c \
		$(MANDATORY_DIR)/routine.c

BONUS_SRCS = 	$(BONUS_DIR)/init_bonus.c \
				$(BONUS_DIR)/main_bonus.c \
				$(BONUS_DIR)/routine_bonus.c \
				$(BONUS_DIR)/utils_bonus.c \
				$(BONUS_DIR)/utils_bonus2.c
				
HEADERS = $(MANDATORY_DIR)/philosophers.h
BONUS_HEADERS = $(BONUS_DIR)/philosophers_bonus.h

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

LIBFT = $(LIBFT_DIR)/libft.a

GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

all: $(NAME)

bonus: $(BONUS_NAME)

$(LIBFT):
	@echo "$(YELLOW)Compilation de libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "$(GREEN)✓ libft compilée$(RESET)"

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(YELLOW)Compilation de $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $@ $(OBJS) -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)✓ Compilation terminée: $(NAME) est prêt$(RESET)"

$(BONUS_NAME): $(LIBFT) $(BONUS_OBJS)
	@echo "$(YELLOW)Compilation de $(BONUS_NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $@ $(BONUS_OBJS) -L$(LIBFT_DIR) -lft
	@echo "$(GREEN)✓ Compilation terminée: $(BONUS_NAME) est prêt$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(MANDATORY_DIR)/%.o: $(MANDATORY_DIR)/%.c $(HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BONUS_DIR)/%.o: $(BONUS_DIR)/%.c $(BONUS_HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW)Suppression des fichiers objets...$(RESET)"
	@$(RM) $(OBJS) $(BONUS_OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(GREEN)✓ Nettoyage effectué$(RESET)"

fclean: clean
	@echo "$(YELLOW)Suppression des exécutables...$(RESET)"
	@$(RM) $(NAME) $(BONUS_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)✓ Nettoyage complet effectué$(RESET)"

re: fclean all

bonus_re: fclean bonus

.PHONY: all bonus clean fclean re bonus_re
