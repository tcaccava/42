# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/06 10:06:03 by girizzi           #+#    #+#              #
#    Updated: 2026/03/15 19:37:46 by girizzi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


GREEN	= \033[0;32m
RED		= \033[0;31m
YELLOW	= \033[0;33m
CYAN	= \033[0;36m
NC		= \033[0m


NAME	= philo

CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -pthread

OBJ_DIR	= obj

SRC		= main.c \
		  init.c \
		  check_args.c \
		  utils.c \
		  threads.c \
		  cleanup.c

OBJ		= $(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(CYAN) COMPILED:$(NC) $<"

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo ""
	@echo "$(GREEN) ✓ $(NAME) COMPILED!$(NC)"
	@echo ""
	@echo " Run: ./philo <N1> <N2> <N3> <N4> <*N5>"
	@echo " N1: Number of philosophers (max 300)"
	@echo " N2: Time to die"
	@echo " N3: Time to eat"
	@echo " N4: Time to sleep"
	@echo " *optional N5: Number of dinners required"
	@echo ""

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(YELLOW) ✓ OBJECTS CLEANED!$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED) ✓ $(NAME) CLEANED!$(NC)"
	@echo ""

re: fclean all

.PHONY: all clean fclean re
