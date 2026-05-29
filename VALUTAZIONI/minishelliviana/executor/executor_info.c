/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_info.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:34:25 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/11 15:34:28 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_info(t_execute *info)
{
	info->com = NULL;
	info->fd[2] = 0;
	info->fd[3] = 1;
	info->pipe = 0;
	info->file = NULL;
	info->args = NULL;
	info->delimiter = 0;
}

void	free_info(t_execute *info)
{
	free_array(info->args);
}

void	print_info(t_execute *info)
{
	int	i;

	i = 0;
	printf("com: %s\n", info->com);
	while (info->args[i])
	{
		printf("arg %d: %s\n", i, info->args[i]);
		i++;
	}
	printf("pipe: %d\n", info->pipe);
	printf("file in: %d\n", info->fd[2]);
	printf("file out: %d\n", info->fd[3]);
	printf("pipe fd: %d\n\n", info->fd[4]);
}
