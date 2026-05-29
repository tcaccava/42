SRC = ft_printf.c printer.c

NAME = libftprintf.a

all: $(NAME)

.c.o: 
	cc -Wall -Wextra -Werror -c $< -o ${<:.c=.o}

OBJS = ${SRC:.c=.o}

$(NAME):	${OBJS}
			ar rcs ${NAME} ${OBJS}

clean:
			rm -f ${OBJS}
			
fclean:		clean
			rm -f ${NAME}

re:			fclean all
