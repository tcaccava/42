NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
RM = rm -f

SRCDIR = srcs
OBJDIR = objs

SRCS = main.c init.c utils.c philosopher.c time.c monitor.c utils_2.c
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re