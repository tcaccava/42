# **************************************************************************** #
#                                   COLORS                                     #
# **************************************************************************** #

GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
NC = \033[0m

# **************************************************************************** #
#                                PROJECT NAME                                  #
# **************************************************************************** #

NAME = philo

# **************************************************************************** #
#                                DIRECTORIES                                   #
# **************************************************************************** #

SRC_DIR = src
INIT_DIR = init_simulation
INC_DIR = includes

# **************************************************************************** #
#                                  SOURCES                                     #
# **************************************************************************** #

SRC =	main.c \
		$(SRC_DIR)/death.c \
		$(SRC_DIR)/fork_routine.c \
		$(SRC_DIR)/routine.c \
		$(SRC_DIR)/routine_status.c \
		$(SRC_DIR)/check_args.c \
		$(INIT_DIR)/monitor.c \
		$(INIT_DIR)/philo_thread.c \
		$(INIT_DIR)/sim_init.c \
		$(INIT_DIR)/sim_run.c

OBJ = $(SRC:.c=.o)

# **************************************************************************** #
#                                COMPILATION                                   #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR)
PTHREAD = -pthread
RM = rm -f

# **************************************************************************** #
#                                   RULES                                      #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(YELLOW)Compiling philo...$(NC)"
	@$(CC) $(CFLAGS) $(PTHREAD) $(OBJ) -o $(NAME)
	@echo "$(GREEN)philo compiled successfully!$(NC)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ)
	@echo "$(RED)Object files removed.$(NC)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(RED)Executable removed.$(NC)"

re: fclean all

.PHONY: all clean fclean re
