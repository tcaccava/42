NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
RM = rm -f
SRCS =	main.c pipe.c parenthesis.c parenthesis2.c ft_cd.c ft_env.c ft_export.c ft_unset.c ft_echo.c ft_exit.c ft_pwd.c utils.c utils2.c utils3.c utils4.c utils5.c utils6.c utils7.c utils8.c find_cmd_path.c signal.c redirect.c wildcards.c wildcards2.c wildcards3.c ft_itoa.c ft_split.c expand_variables.c handling.c tokenize.c parse.c tree.c signal2.c exec_cmd.c exec_extern_cmd.c free.c
OBJS = $(SRCS:.c=.o)
all : $(NAME)
$(NAME) : $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
clean :
	$(RM) $(OBJS)
fclean: clean
	$(RM) $(NAME)
re : fclean all
