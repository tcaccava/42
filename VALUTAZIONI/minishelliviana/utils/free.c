/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:21:09 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/20 16:16:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **str)
{
	int	n;

	n = 0;
	while (str && str[n])
	{
		free (str[n]);
		n++;
	}
	free(str);
}

void	exit_and_free(t_data *data, t_execute *info)
{
	int	exit_status;

	close(info->fd[0]);
	close(info->fd[1]);
	exit_status = data->exit_status;
	free_array(info->args);
	free(info->fd);
	free(info);
	free_array(data->env);
	free_tokens(data->tokens);
	free(data);
	exit(exit_status);
}

void	final_free(t_execute *info, t_data *data)
{
	int	exit_status;

	exit_status = data->exit_status;
	close(info->fd[0]);
	close(info->fd[1]);
	rl_clear_history();
	free_array(data->env);
	free(info->fd);
	free(info);
	free(data);
	exit(exit_status);
}
