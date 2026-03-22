
# Compiler
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Directories
SRC_DIR = srcs
INC_DIR = includes
OBJ_DIR = srcs

# Sources
SRC = $(SRC_DIR)/ft_putchar.c \
      $(SRC_DIR)/ft_swap.c \
      $(SRC_DIR)/ft_putstr.c \
      $(SRC_DIR)/ft_strlen.c \
      $(SRC_DIR)/ft_strcmp.c

# Objects: same folder as sources
OBJ = $(SRC:.c=.o)

# Library name
NAME = libft.a

# Default rule
all: $(NAME)

# Rule to build the library
$(NAME): $(OBJ)
	@echo "Creating library $@..."
	ar rcs $@ $(OBJ)

# Compile .c to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

# Remove object files
clean:
	@echo "Cleaning object files..."
	rm -f $(OBJ)

# Remove object files + library
fclean: clean
	@echo "Removing library..."
	rm -f $(NAME)

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re
