# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pspaccas <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/25 19:34:57 by pspaccas          #+#    #+#              #
#    Updated: 2025/06/25 19:45:13 by pspaccas         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                               MAKEFILE - PUSH_SWAP                          #
# **************************************************************************** #

NAME		= push_swap

# ************************************************************************** #
#                                COMPILER                                   #
# ************************************************************************** #
CC			= cc
CFLAGS		= -Wall -Wextra -Werror

# ************************************************************************** #
#                               LIBRARIES                                    #
# ************************************************************************** #
LIBFT_DIR	= lib/libft
GNL_DIR		= lib/get_next_line
PRINTF_DIR	= lib/libftprintf

LIBFT		= $(LIBFT_DIR)/libft.a
PRINTF		= $(PRINTF_DIR)/libftprintf.a

# ************************************************************************** #
#                                FILES                                       #
# ************************************************************************** #

INCLUDES	= -I include -I $(LIBFT_DIR) -I $(PRINTF_DIR) -I $(GNL_DIR)

SRC_DIR		= srcs
SRC			= $(shell find $(SRC_DIR) -name "*.c")
OBJ			= $(SRC:.c=.o)

# ************************************************************************** #
#                                RULES                                       #
# ************************************************************************** #

all: $(NAME)

$(NAME): $(LIBFT) $(PRINTF) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(PRINTF) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(PRINTF):
	@$(MAKE) -C $(PRINTF_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(MAKE) clean -C $(LIBFT_DIR)
	@$(MAKE) clean -C $(PRINTF_DIR)
	rm -f $(OBJ)

fclean: clean
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@$(MAKE) fclean -C $(PRINTF_DIR)
	rm -f $(NAME)

re: fclean all

bonus:
	@$(MAKE) BONUS=1

.PHONY: all clean fclean re bonus
