/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 02:11:34 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/13 15:18:18 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	setup_heredoc_execution(int *pipefd, char **argv, char **envp)
{
	int		fd_out;
	char	**cmd;
	char	*command_path;

	fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	close(pipefd[1]);
	dup2(pipefd[0], 0);
	dup2(fd_out, 1);
	close(pipefd[0]);
	close(fd_out);
	cmd = ft_split(argv[3], ' ');
	command_path = find_path(cmd[0], envp);
	execve(command_path, cmd, envp);
	return (1);
}

int	execute_heredoc(char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid;

	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		here_doc(argv[2], pipefd);
		exit(0);
	}
	else
	{
		return (setup_heredoc_execution(pipefd, argv, envp));
	}
	return (0);
}

int	handle_normal(t_pipex *px, char **argv, char **envp, int argc)
{
	px->fd1 = ft_open(argv[1], O_RDONLY, 0);
	px->fd2 = ft_open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	px->cmd_count = argc - 3;
	multiple_pipes(px, argv, envp);
	if (px->fd1 != -1)
		close(px->fd1);
	if (px->fd2 != -1)
		close(px->fd2);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	px;

	if (argc < 5)
	{
		write(2, "there are less than 4 arguments\n", 31);
		return (1);
	}
	px.cmd_count = argc - 3;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		return (execute_heredoc(argv, envp));
	return (handle_normal(&px, argv, envp, argc));
}
