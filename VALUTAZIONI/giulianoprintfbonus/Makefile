CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = libftprintf.a

BONUS_DIR = bonus
BONUS_NAMES = conv_spec_parser_bonus t_flags_data_utils specifier_d_i_u specifier_c_s specifier_p_x_X print_utils number_utils exec_if
SRCS = src/ft_printf.c
BASE_SRCS = base/conv_spec_parser.c base/specifier_printer.c
BONUS_SRCS = $(addprefix $(BONUS_DIR)/,$(addsuffix .c,$(BONUS_NAMES)))

OBJS = $(SRCS:.c=.o) $(BASE_SRCS:.c=.o)
BONUS_OBJS = $(SRCS:.c=.o) $(BONUS_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(BONUS_OBJS)
	ar rcs $(NAME) $(BONUS_OBJS)

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
