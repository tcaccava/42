/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 23:19:52 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/13 11:10:33 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	here_doc(char *limiter, int *pipefd)
{
	char	*line;

	close(pipefd[0]);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(0);
		if (!line)
		{
			perror("get_next_line");
			break ;
		}
		ft_printf("Input received: %s\n", line);
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipefd[1]);
}

void	child_process(t_pipex *px, char *cmd, int *pipefd, char **envp)
{
	if (px->fd1 != -1)
		dup2(px->fd1, 0);
	px->fd1 = pipefd[0];
	dup2(pipefd[1], 1);
	close(pipefd[0]);
	close(pipefd[1]);
	ft_exec(cmd, envp);
}

void	last_child_process(t_pipex *px, char *cmd, int *pipefd, char **envp)
{
	dup2(pipefd[0], 0);
	if (px->fd2 != -1)
		dup2(px->fd2, 1);
	close(pipefd[0]);
	close(pipefd[1]);
	ft_exec(cmd, envp);
}

void	multiple_pipes(t_pipex *px, char **argv, char **envp)
{
	int		i;
	int		pipefd[2];
	pid_t	pid;

	i = 2;
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
		child_process(px, argv[i], pipefd, envp);
	close(pipefd[1]);
	px->fd1 = pipefd[0];
	i++;
	while (i < px->cmd_count + 1)
	{
		pipe(pipefd);
		pid = fork();
		if (pid == 0)
			child_process(px, argv[i], pipefd, envp);
		close(pipefd[1]);
		px->fd1 = pipefd[0];
		i++;
	}
	last_child_process(px, argv[i], pipefd, envp);
}
