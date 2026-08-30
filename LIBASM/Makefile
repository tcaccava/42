NAME        = libasm.a

ASM         = nasm
ASMFLAGS    = -f elf64

AR          = ar rcs
RM          = rm -f

SRCS        = ft_strlen.s ft_strcpy.s ft_strcmp.s ft_write.s ft_read.s ft_strdup.s
BONUS       = ft_list_push_front_bonus.s ft_list_size_bonus.s ft_list_sort_bonus.s ft_list_remove_if_bonus.s ft_atoi_base_bonus.s

OBJS        = $(SRCS:.s=.o)
BOBJS       = $(BONUS:.s=.o)

%.o: %.s
	$(ASM) $(ASMFLAGS) $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

bonus: $(OBJS) $(BOBJS)
	$(AR) $(NAME) $(OBJS) $(BOBJS)

clean:
	$(RM) $(OBJS) $(BOBJS)

fclean: clean
	$(RM) $(NAME) a.out 

re: fclean all

.PHONY: all clean fclean re bonus