NAME	= libftprintf.a

SRCS = ft_printf.c \
       ft_putchar.c \
       ft_putstr.c \
       ft_putnbr.c \
       ft_putunsigned.c \
       ft_puthex.c \
       ft_putpointer.c

OBJS	= $(SRCS:.c=.o)

CC	= gcc

CFLAGS	= -Wall -Wextra -Werror

RM	= rm -f

all:	$(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
