NAME		= libasm.a

NASM		= nasm
NASMFLAGS	= -f elf64
CC		= cc
CFLAGS		= -Wall -Wextra -Werror

OBJDIR		= obj

SRCS		= ft_strlen.s \
		  ft_strcpy.s \
		  ft_strcmp.s \
		  ft_write.s \
		  ft_read.s \
		  ft_strdup.s

OBJS		= $(addprefix $(OBJDIR)/, $(SRCS:.s=.o))

all:		$(NAME)

$(NAME):	$(OBJS)
		ar rcs $(NAME) $(OBJS)

$(OBJDIR)/%.o:	%.s | $(OBJDIR)
		$(NASM) $(NASMFLAGS) $< -o $@

$(OBJDIR):
		mkdir -p $(OBJDIR)

test:		$(NAME) main.c
		$(CC) $(CFLAGS) main.c -L. -lasm -o test_libasm
		./test_libasm

clean:
		rm -rf $(OBJDIR)

fclean:		clean
		rm -f $(NAME) test_libasm

re:		fclean all

.PHONY:		all clean fclean re test
