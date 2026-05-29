/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:20:11 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/17 20:20:12 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_fd(t_execute *info)
{
	if (info->fd[2] != 0)
	{
		dup2(info->fd[2], STDIN_FILENO);
		close(info->fd[2]);
	}
	if (info->fd[3] != 1)
	{
		dup2(info->fd[3], STDOUT_FILENO);
		close(info->fd[3]);
	}
}

void	restore_fd(t_execute *info)
{
	dup2(info->fd[0], STDIN_FILENO);
	dup2(info->fd[1], STDOUT_FILENO);
}

void	close_fd(int fd1, int fd2, int fd3)
{
	if (fd1 > 2)
		close(fd1);
	if (fd2 > 2)
		close(fd2);
	if (fd3 > 2)
		close(fd3);
}
