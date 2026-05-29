/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:30:55 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/23 18:06:08 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stddef.h>
# include <dirent.h>
# include <fcntl.h>
# include <sys/types.h>

# include "ft_printf.h"
# include "get_next_line.h"
# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

extern int	g_last_signal;

typedef enum e_type
{
	NONE = 0,
	CMD = 1 << 1,
	LIMITER = 1 << 2,
	DELIMETER = 1 << 3,
	REDIRECT = 1 << 4,
	FILENAME = 1 << 5,
	AND = 1 << 6,
	OR = 1 << 7,
	IN = 1 << 8,
	OUT = 1 << 9,
	HEREDOC = 1 << 10,
	APPEND = 1 << 11,
	PIPE = 1 << 12,
	OPEN = 1 << 13,
	CLOSE = 1 << 14,
	NEW_LINE = 1 << 15,
	END = 1 << 16,
}	t_type;

typedef struct s_token
{
	int				index;
	void			*content;
	t_type			type;
	t_type			sub_type;
	int				quotes;
	struct s_token	**args;
	struct s_token	*left;
	struct s_token	*right;
}	t_token;

typedef struct s_data
{
	t_token		**tokens;
	char		**variables;
	t_token		*root;
	char		**env;
	int			exit_status;
	t_token		*tree;
	char		*files;
	int			tokens_result;
}	t_data;

typedef struct s_execute
{
	char		*com;
	char		**args;
	int			pipe;
	int			processes;
	pid_t		pid;
	t_type		delimiter;
	char		*file;
	int			*fd;
	char		**to_unlink;
}				t_execute;

//utils.c
int		ft_strcmp(char *s1, char *s2);

// tokens_utils.c && tokens_utils2.c
t_token	**remove_token_at(t_token **arr, size_t index);
t_token	**add_token_at(t_token **arr, t_token *token, size_t index);
t_token	*create_token(void *content, t_type type, t_type sub_type);
t_token	**add_token(t_token **arr, t_token *token);
void	copy_arr(t_token **new_arr, t_token **arr, size_t size);
void	free_tokens(t_token **tokens);
size_t	count_tokens(t_token **tokens);
void	print_tokens(t_token **tokens);
int		free_token(t_token *token);

//tokenizer_utils.c
int		special_char(char c);
char	*create_str(char *line, size_t i, size_t j);
int		search_special(char *line, size_t i);
void	jump_spaces(char *line, size_t *i, size_t *j);

//tokenizer.c
int		tokenizer(char *line, t_token ***tokens);

//tokens_reoder.c
void	reorder_tokens(t_token **tokens);
void	assign_index(t_token **tokens);

t_token	*build_command_tree(t_token **tokens, int token_count);
void	print_command_tree(t_token *node, int depth);

//tree_builder.c
t_token	*build_tree(t_token **tokens, size_t *i);

//echo.c
void	ft_echo(char **str, t_data *data, t_execute *info);

//cd.c
void	cd(char **str, t_data *data, t_execute *info);

//pwd.c
void	pwd(t_data *data, t_execute *info);
void	update_pwd(char *new_dir, t_execute *info, t_data *data);

//env.c
void	ft_env(t_data *data, t_execute *info);
t_data	*analize_env(char **env);

//export.c
void	ft_export(char **var, t_data *data, t_execute *info);
void	change_env(t_data *data, char *var, int i, int n);
void	add_env(char *var, t_data *data);

//exit.c
void	ft_exit(char **exit_status, t_data *data, t_execute *info);

//unset.c
void	ft_unset(char **var, t_data *data);

//error.c
void	check_error(int n, char *comm, char *arg, t_data *data);
void	command_error(char *comm, t_data *data, t_execute *info);
void	check_dup(int n, t_execute *info, t_data *data, int fd);
void	file_error(int n, t_execute *info, t_data *data);

//free.c
void	free_array(char **str);
void	exit_and_free(t_data *data, t_execute *info);
void	final_free(t_execute *info, t_data *data);

//utils.c
char	**copy_array(char **array);
int		array_len(char **array);
char	**add_array(t_data *data, char *var);
char	**sort_array(char **env);

//ft_getenv.c
char	*ft_getenv(char *variable, char **env);
char	*get_value(char *str);

//wildcards.c
char	**find_wildcards(char *str);
int		len_wildcards(char *str);

//executor.c
void	executor(t_token *tree, t_data *data, t_execute *info);
int		check_builtin(t_execute *info, t_data *data);

//path.c
char	*findpath(char **env, char *com);

//executor_info.c
void	print_info(t_execute *info);
void	set_info(t_execute *info);

//pipex.c
void	execve_cmd(t_execute *info, t_data *data);

//set_prompt.c
char	*set_prompt(void);

//fd_utils.c
void	set_fd(t_execute *info);
void	restore_fd(t_execute *info);
void	close_fd(int fd1, int fd2, int fd3);

//export_utils.c
char	*export_join(char *str);
char	*get_export_variable(char *str);
char	*ft_strjoin2(char *s1, char *s2);
int		array_len_norm(char **array);

//ft_arrayjoin.c
char	**ft_arrayjoin(char **s1, char **s2);

//initial_configuration.c
void	initial_configuration(t_data *data, t_execute *info);
void	start_execution(t_execute *info, t_data *data);

char	*ft_strdup_quote(char *str);

int		assign_args(t_token **tokens);
void	print_args(t_token **tokens);

//int    write_on_file(int fd, char *delimeter);
//int     check_heredoc(t_token **tokens);
int		finalize_tokens(t_token **tokens, t_data *data, t_execute *info);
int		forbidden_symbols(char c);

int		syntax_error(t_token **tokens, t_token *check);

t_token	*check_args(t_token **tokens);

//signal_handlers.c
void	sigint_handler(int signum);
void	sigquit_handler(int signum);
void	setup_signal_handlers(void);
void	heredoc_handler(int signum);
void	signal_manager(int signum, void (*handler)(int s));

//quotes_utils.c
void	remove_quotes(char *line);
t_token	*check_quotes(t_token *token, t_token **res);

char	*check_export2(char *line, t_data *data);

//check_next.c
t_token	*check_next(t_token **tokens, size_t i, t_token **res);

//create_special.c
t_token	*create_special(char *line, size_t *i, size_t *j);

//create_cmd.c
t_token	*create_cmd(char *line, size_t *i, size_t *j, t_type type);

//select_handler.c
void	select_handler(t_token **tokens, t_token **root,
			t_token **last, size_t *i);

//write_on_file.c
int		write_on_file(int fd, char *delimiter, t_token **tokens, t_data *data);

//check_here
int		check_heredoc(t_token **tokens, size_t *i, t_data *data, \
t_execute *info);

//tokens_final_funcs_utils.c
int		forbidden_symbols(char c);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*replace_range(char *s1, char *s2, size_t i, size_t j);

//process_string.c
char	*process_string(char *line, t_data *data, long *j, long *k);

//temp_files_utils.c
void	unlink_files(t_data *data);
#endif
