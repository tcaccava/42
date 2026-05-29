/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:35:23 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/04/19 16:09:42 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unset(char **var, t_data *data)
{
	int			i;
	int			len;
	extern char	**environ;
	char		*to_delete;

	i = 0;
	if ((*var))
	{
		to_delete = ft_getenv(*var, data->env);
		if (to_delete)
		{
			len = ft_strlen(*var);
			while (ft_strncmp((data->env)[i], *var, len) != 0 && \
			(data->env[i][len + 1] != '=' || data->env[i][len + 1] != '\0'))
				i++;
			free(to_delete);
			to_delete = data->env[i];
			data->env[i] = ft_strdup("");
			environ = copy_array(data->env);
			free_array(data->env);
			data->env = environ;
		}
		free(to_delete);
	}
	data->exit_status = 0;
}
