NAME = cat
SRCS = cat.c
CC = gcc
RM = rm -f
OBJ = $(SRCS:.c = .o)
CFLAGS = -Wall -Wextra -Werror
all : $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
clean : 
	$(RM) $(OBJ)
fclean : clean
	$(RM) $(NAME)
re : fclean all

