/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial_configuration.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 12:18:05 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/23 17:22:23 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_bash_level(t_data *data, t_execute *info);

void	initial_configuration(t_data *data, t_execute *info)
{
	extern char	**environ;

	setup_signal_handlers();
	data->env = copy_array(environ);
	info->pid = 0;
	info->processes = 0;
	info->fd = malloc(5 * sizeof(int *));
	info->fd[0] = dup(STDIN_FILENO);
	info->fd[1] = dup(STDOUT_FILENO);
	info->fd[4] = 0;
	data->exit_status = 0;
	data->tokens_result = 0;
	set_bash_level(data, info);
	environ = data->env;
}

void	set_bash_level(t_data *data, t_execute *info)
{
	char	*bash_level;
	int		new_level;
	char	**temp;

	bash_level = ft_getenv("SHLVL", data->env);
	if (!bash_level)
		return ;
	new_level = ft_atoi(bash_level) + 1;
	free(bash_level);
	bash_level = malloc(8);
	ft_strlcpy(bash_level, "SHLVL=", 7);
	bash_level = ft_strjoin2(bash_level, ft_itoa(new_level));
	temp = ft_split(bash_level, ' ');
	free (bash_level);
	ft_export(temp, data, info);
	free_array(temp);
}

void	start_execution(t_execute	*info, t_data *data)
{
	set_info(info);
	executor(data->tree, data, info);
	execve_cmd(info, data);
	waitpid(info->pid, &(data->exit_status), 0);
	if (WIFEXITED(data->exit_status))
		data->exit_status = WEXITSTATUS(data->exit_status);
	while (info->processes > 0)
	{
		wait(NULL);
		info->processes -= 1;
	}
	free_array(info->args);
}
