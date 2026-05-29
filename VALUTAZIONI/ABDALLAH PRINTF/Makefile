NAME = libftprintf.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = ft_printf.c \
      ft_lenhex.c \
      ft_lenptr.c \
      ft_lenunbr.c \
      ft_printhex.c \
      ft_printptr.c \
      ft_putchar.c \
      ft_puthex.c \
      ft_putnbr.c \
      ft_putptr.c \
      ft_putstr.c \
      ft_putunbr.c\
      ft_putchar_fd.c\
      ft_putnbr_fd.c\
      ft_putstr_fd.c\
      ft_isdigitt.c\
      ft_toupperr.c\
      ft_itoaa.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
