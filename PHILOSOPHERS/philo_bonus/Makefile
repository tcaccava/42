NAME = philo_bonus
CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
SRCS = bonus.c bonus2.c bonus3.c ft_atoi.c main.c
OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re: fclean all
