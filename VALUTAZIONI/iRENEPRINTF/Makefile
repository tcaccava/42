NAME = libftprintf.a
CC = cc
AR = ar rcs
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

SOURCES = ft_count_low_hex.c\
	  ft_count_putint.c\
	  ft_count_unint.c\
	  ft_printf.c\
	  ft_count_ptr.c\
	  ft_count_putnbr_base.c\
	  ft_count_up_hex.c\
	  ft_count_putchar.c\
	  ft_count_putstr.c\
	  ft_formatter.c\
	  ft_strlen.c

OBJECTS = $(SOURCES:.c=.o)

$(NAME): $(OBJECTS)
		$(AR) $@ $^

%.o: %.c
			$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
			$(RM) $(OBJECTS)

fclean: clean
			$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
