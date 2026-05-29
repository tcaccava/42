NAME            =   libftprintf.a

CC              =   gcc
CFLAGS          =   -Wall -Wextra -Werror
AR              =   ar
ARFLAGS         =   rcs
RM              =   rm -rf

SRC_DIR         =   utils
SRC             =   $(SRC_DIR)/ft_printf $(SRC_DIR)/ft_putchar $(SRC_DIR)/ft_putstr \
                    $(SRC_DIR)/ft_putnbr_u $(SRC_DIR)/ft_putnbr \
                    $(SRC_DIR)/ft_putnbrhex_x $(SRC_DIR)/ft_putptr \
                    $(SRC_DIR)/ft_putnbrhex_X

SRCS            =   $(addsuffix .c, $(SRC))

OBJ_DIR         =   obj
OBJS            =   $(SRCS:%.c=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $(NAME) $(OBJS)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

