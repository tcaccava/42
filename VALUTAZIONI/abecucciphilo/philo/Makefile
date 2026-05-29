NAME = philo





GREEN = \033[32m
RED = \033[31m
RESET = \033[0m
CANCEL = \033[1A\033[K





CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -gdwarf-4 -pthread

MAKEFLAGS += -s





SRCSDIR = ./
SRCS =	main.c					\
		init_shared.c			\
		init_philos.c			\
		start_simulation.c		\
		monitor.c				\
		philosophers_loop.c		\
		utils.c					\
		cleanup.c				\


OBJDIR = $(SRCSDIR)/obj
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))





$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	printf "\n$(CANCEL)"
	printf "$(GREEN)$(NAME) compiled!\n$(RESET)"

$(OBJDIR)/%.o: $(SRCSDIR)/%.c
	mkdir -p $(dir $@)
	printf "$(GREEN)Generating $(NAME) objects... %-33.33s\r$(RESET)" $@
	$(CC) $(CFLAGS) -c $< -o $@





all: $(NAME)

clean:
	rm -rf $(OBJS)
	printf "$(RED)$(NAME) objects deleted!$(RESET)\n"

fclean:
	rm -rf $(NAME)
	rm -rf $(OBJDIR)
	printf "$(RED)$(NAME) and objects deleted!$(RESET)\n"

re: fclean all

bonus: all





.PHONY: all clean fclean re bonus
