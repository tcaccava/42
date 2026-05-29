SRC = main.cpp BitcoinExchange.cpp

OBJDIR = objects
OSRC = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))

CFLAGS = -Wall -Wextra -Werror -std=c++98
CC = c++

NAME = btc 

BLUE = \033[1;34m
RESET = \033[0m

all: $(NAME)

$(NAME): ascii_art $(OSRC)
	@$(CC) $(CFLAGS) $(OSRC) -o $@

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@printf "$(GREEN)Compiling %-20s\033[0m\r" $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

ascii_art:
	@printf "$(BLUE)"
	@cat ascii.txt
	@echo
	@printf "$(RESET)"

.PHONY: all clean fclean re ascii_art
