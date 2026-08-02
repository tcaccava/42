# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbardett <mbardett@student.42roma.it>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/31 18:53:56 by mbardett          #+#    #+#              #
#    Updated: 2026/07/31 19:08:27 by mbardett         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


RESET = \033[0m
GREEN = \033[38;5;46m
CYAN = \033[38;5;51m
ORANGE = \033[38;5;202m


NAME = libasm.a
SRC = ft_strlen.s ft_strcpy.s ft_strcmp.s ft_write.s ft_read.s ft_strdup.s
OBJ = $(SRC:.s=.o)

AS = nasm
ASFLAGS = -f elf64
AR = ar
ARFLAGS = rcs
CC = gcc
CFLAGS = -Wall -Wextra -Werror

%.o: %.s
	@$(AS) $(ASFLAGS) $< -o $@

$(NAME): $(OBJ)
	@$(AR) $(ARFLAGS) $(NAME) $(OBJ)
	@echo "$(GREEN) $(NAME) built $(RESET)" 

all: $(NAME)

test: all
	@$(CC) $(CFLAGS) main.c -L. -lasm -o test_asm
	@echo "$(CYAN)MAKEFILE: test_asm built$(RESET)"

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME) test_asm

re: fclean all

.PHONY: all clean fclean re test