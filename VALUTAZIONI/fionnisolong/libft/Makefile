FILES =		ft_memset.c \
		ft_bzero.c \
		ft_memmove.c \
		ft_memcpy.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_strlen.c \
		ft_strlcpy.c \
		ft_strlcat.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_strncmp.c \
		ft_strnstr.c \
		ft_atoi.c \
		ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_strdup.c \
		ft_calloc.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_strtrim.c \
		ft_strmapi.c \
		ft_split.c \
		ft_itoa.c \
		ft_putchar_fd.c \
		ft_putnbr_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_striteri.c \

FILES_B = ft_lstnew.c \
		  ft_lstadd_front.c \
		  ft_lstsize.c \
		  ft_lstlast.c \
		  ft_lstiter.c \
		  ft_lstdelone.c \
		  ft_lstclear.c \
		  ft_lstmap.c \
		  ft_lstadd_back.c \

NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f
OBJFILES = $(FILES:.c=.o)
BOBJFILES = $(FILES_B:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: ${NAME}

$(NAME): ${OBJFILES}
	$(AR) $(NAME) $(OBJFILES)

bonus: ${NAME} ${BOBJFILES}
	$(AR) $(NAME) $(BOBJFILES)

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME) $(OBJFILES)

re: fclean all
.PHONY: all clean fclean re bonus