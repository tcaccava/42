/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 16:32:53 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/19 17:52:29 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execution_error(t_execute *info, t_data *data, \
char *path, char *com_flags)
{
	perror(com_flags);
	data->exit_status = 126;
	free(path);
	exit_and_free(data, info);
}

void	execution(t_execute *info, t_data *data)
{
	char	*path;
	char	**com_flags;

	close(info->fd[0]);
	close(info->fd[1]);
	if (check_builtin(info, data) == 1)
		exit_and_free(data, info);
	com_flags = info->args;
	if (!*(info->com))
		command_error(com_flags[0], data, info);
	path = findpath(data->env, com_flags[0]);
	if (!path)
	{
		if (*com_flags[0] == '.' || *com_flags[0] == '/')
		{
			if (execve(com_flags[0], com_flags, data->env) == -1)
				command_error(com_flags[0], data, info);
		}
		else
			command_error(com_flags[0], data, info);
	}
	execve(path, com_flags, data->env);
	execution_error(info, data, path, com_flags[0]);
}

void	execute_pipe(t_execute *info, t_data *data)
{
	int		pipefd[2];
	pid_t	pid;

	info->processes += 1;
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		check_dup(dup2(info->fd[2], STDIN_FILENO), info, data, pipefd[1]);
		if (info->fd[3] < 2)
			dup2(pipefd[1], STDOUT_FILENO);
		else
			dup2(info->fd[3], STDOUT_FILENO);
		close_fd(info->fd[3], 0, 0);
		close_fd(info->fd[2], pipefd[1], pipefd[0]);
		execution(info, data);
	}
	close_fd(pipefd[1], info->fd[4], info->fd[3]);
	info->fd[4] = pipefd[0];
}

void	final_process(t_execute *info, t_data *data)
{
	pid_t	pid;

	if (!ft_strncmp(info->args[0], "./", 2))
		signal_manager(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		check_dup(dup2(info->fd[2], STDIN_FILENO), info, data, 0);
		dup2(info->fd[3], STDOUT_FILENO);
		close_fd(info->fd[2], info->fd[3], 0);
		execution(info, data);
	}
	info->pid = pid;
	if (info->fd[2] > 0)
		close(info->fd[2]);
}

void	execve_cmd(t_execute *info, t_data *data)
{
	if (info->fd[2] != -2)
		file_error(info->fd[2], info, data);
	if (info->fd[2] == -2 || info->com == NULL)
	{
		close_fd(info->fd[2], info->fd[3], 0);
		return ;
	}
	if ((info->delimiter == AND && data->exit_status != 0) || \
	(info->delimiter == OR && data->exit_status == 0))
		return ;
	if (info->fd[4] == 0 && info->pipe == 0)
		if (check_builtin(info, data) == 1)
			return ;
	if (info->fd[4] != 0)
	{
		if (info->fd[2] == 0)
			info->fd[2] = info->fd[4];
		info->fd[4] = 0;
	}
	if (info->pipe == 0)
		final_process(info, data);
	else
		execute_pipe(info, data);
	close_fd(info->fd[2], info->fd[3], 0);
}
