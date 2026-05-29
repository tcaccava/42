NAME = libftprintf.a

SRC = ft_printf.c ft_pointr.c ft_putchar.c ft_puthex.c ft_putnbr.c ft_putstr.c ft_unsigned.c 
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

