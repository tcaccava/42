NAME = minishell

PARSER = ./parser/

builtin = ./builtin/

utils = ./utils/

executor = ./executor/

SRC = main.c $(builtin)echo.c $(builtin)cd.c $(utils)error.c $(utils)free.c $(builtin)pwd.c $(builtin)export.c \
      $(builtin)exit.c $(builtin)env.c $(utils)array_utils.c $(utils)ft_getenv.c $(builtin)unset.c $(executor)wildcards.c \
      $(PARSER)tokens_utils.c $(PARSER)tokens_utils2.c $(PARSER)tokens_reorder.c $(PARSER)tree_builder.c \
      $(PARSER)tokenizer.c $(PARSER)token_args_utils.c $(PARSER)tokens_final_funcs.c $(executor)executor.c \
      $(executor)path.c $(executor)executor_info.c $(executor)pipex.c $(PARSER)check_input.c $(utils)errors_utils.c \
      $(utils)set_prompt.c signal_handlers.c $(utils)ft_arrayjoin.c $(executor)initial_configuration.c $(PARSER)quotes_utils.c\
      $(utils)fd_utils.c $(utils)export_utils.c $(PARSER)check_next.c $(PARSER)create_special.c $(PARSER)create_cmd.c \
      $(PARSER)tokenizer_utils.c $(PARSER)select_handler.c $(PARSER)write_on_file.c $(PARSER)check_heredoc.c \
      $(PARSER)tokens_final_funcs_utils.c $(PARSER)process_string.c $(utils)temp_files_utils.c $(utils)utils.c

OBJ = $(SRC:.c=.o)

CC = cc

CFLAGS = -Wall -Werror -Wextra -g -gdwarf-4

INCLUDE = -Ilibft/header_files -I/usr/include -I./includes

LIBFLAGS = -Llibft -lft -L/usr/lib

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft
	@$(CC) $(OBJ) $(LIBFLAGS) -o $(NAME) -lreadline -lncurses -g
	@echo "compilation done :)"

%.o:%.c
	@echo "compiling $@"
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	rm -f $(OBJ)
	make clean -C libft

fclean:
	rm -f $(OBJ) $(NAME)
	make fclean -C libft

re: fclean $(NAME)

.PHONY: all clean fclean re
