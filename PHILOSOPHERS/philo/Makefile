NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = philo.c philo2.c philo3.c philo4.c main.c ft_atoi.c
OBJS = $(SRCS:.c=.o)
RM = rm -f
all : $(NAME)
$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean :
	$(RM) $(OBJS)
fclean : clean
	$(RM) $(NAME)
re: fclean all