/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 15:09:33 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/11 15:09:36 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_data *data, t_execute *info)
{
	int		i;
	char	*value;

	i = 0;
	set_fd(info);
	while (data->env[i])
	{
		value = ft_getenv(data->env[i], data->env);
		if (*value != '\0')
		{
			ft_putstr_fd(data->env[i], STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		i++;
		free (value);
	}
	data->exit_status = 0;
	restore_fd(info);
}

t_data	*analize_env(char **env)
{
	t_data	*env_variables;
	int		n;
	int		i;

	env_variables = malloc(sizeof(t_data));
	env_variables->variables = malloc((array_len(env) + 1) * sizeof(char *));
	n = 0;
	i = 0;
	while (env[n])
	{
		i = 0;
		while (env[n][i] != '=' && env[n][i] != '\0')
			i++;
		env_variables->variables[n] = ft_substr(env[n], 0, i);
		printf("%s\n", env_variables->variables[n]);
		n++;
	}
	env_variables->variables[n] = NULL;
	return (env_variables);
}
