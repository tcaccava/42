/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 02:12:04 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/13 14:22:17 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42

# endif

typedef struct s_pipex
{
	int		fd1;
	int		fd2;
	char	*cmd1;
	char	*cmd2;
	int		cmd_count;
}			t_pipex;

char		**ft_split(const char *str, char delimiter);
char		**split_words(const char *str, char delimiter, char **split,
				int *i);
void		ft_exec(char *cmd, char **envp);
void		pipex(t_pipex *px, char **envp);
void		child_process1(t_pipex *px, int *pipefd, char **envp);
void		child_process2(t_pipex *px, int *pipefd, char **envp);
int			ft_open(char *file, int flags, int mode);
char		**get_paths(char **envp);
char		*concat_path(char *dir, char *cmd);
char		*find_path(char *cmd, char **envp);
size_t		ft_strlen(const char *str);
char		*ft_strcpy(char *dest, const char *src);
char		*ft_strcat(char *dest, const char *src);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*get_next_line(int fd);
int			read_join(int fd, char **loot);
char		*extract_line(char *loot);
char		*clear_loot(char *loot);
char		*ft_strdup(const char *s);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_strchr(const char *s, int c);
void		last_child_process(t_pipex *px, char *cmd, int *pipefd,
				char **envp);
void		multiple_pipes(t_pipex *px, char **argv, char **envp);
int			handle_normal(t_pipex *px, char **argv, char **envp, int argc);
int			handle_heredoc(t_pipex *px, char **argv, char **envp, int argc);
void		here_doc(char *limiter, int *pipefd);
void		first_child_process(t_pipex *px, char *cmd, int *pipefd,
				char **envp);
void		child_process(t_pipex *px, char *cmd, int *pipefd, char **envp);
void		intermediate_child_process(t_pipex *px, char *cmd, int *pipefd,
				char **envp);
int			execute_heredoc(char **argv, char **envp);
int			setup_heredoc_execution(int *pipefd, char **argv, char **envp);
void		ft_formatspecifier(char specifier, va_list args, int *length);
int			ft_printf(const char *print, ...);
void		ft_putchar(char c, int *length);
void		ft_putstr(char *s, int *length);

#endif