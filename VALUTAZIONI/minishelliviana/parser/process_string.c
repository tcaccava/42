/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:37:26 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 17:37:33 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	move_index(char *line, long *j)
{
	while (line[*j] && !forbidden_symbols(line[*j]))
		(*j)++;
	return (*j);
}

static char	*replace_env_var(char *line, long k, long j, t_data *data)
{
	char	*sub_str;
	char	*var;

	sub_str = create_str(line, k, j - 1);
	var = ft_getenv(sub_str, data->env);
	if (!var)
		var = ft_strdup("");
	free(sub_str);
	line = replace_range(line, var, k - 1, j - 1);
	return (line);
}

static char	*replace_exit_status(char *line, long k, long j, t_data *data)
{
	char	*var;

	var = ft_itoa(data->exit_status);
	return (replace_range(line, var, k - 1, j));
}

char	*process_string(char *line, t_data *data, long *j, long *k)
{
	*k = *j + 1;
	*j = *k;
	if (line[*j] && line[*j] != '?')
		move_index(line, j);
	else if (!line[*j] || !forbidden_symbols(line[*j]))
		return (NULL);
	if (*j != *k)
		line = replace_env_var(line, *k, *j, data);
	else if (line[*j] == '?' && line[*k] == '?')
		line = replace_exit_status(line, *k, *j, data);
	else if (line[*j] == line[*k])
	{
		(*j)++;
		(*k)++;
	}
	*j = -1;
	return (line);
}
