/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:37:14 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/27 04:33:55 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_env_string(const char *name, const char *value)
{
	char	*new_var;

	new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (!new_var)
		return (NULL);
	sprintf(new_var, "%s=%s", name, value);
	return (new_var);
}

int	update_env_array(char **envp, int count, const char *name,
		const char *value)
{
	char	**new_envp;
	char	*new_var;
	int		i;

	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (1);
	i = 0;
	while (i < count)
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_var = create_env_string(name, value);
	if (!new_var)
	{
		free(new_envp);
		return (1);
	}
	new_envp[count] = new_var;
	new_envp[count + 1] = NULL;
	memcpy(envp, new_envp, sizeof(char *) * (count + 2));
	free(new_envp);
	return (0);
}

int	set_env_var(char **envp, const char *name, const char *value)
{
	int		i;
	int		count;
	size_t	len_name;
	char	*new_var;

	i = 0;
	count = 0;
	len_name = ft_strlen(name);
	while (envp[count])
		count++;
	while (i < count)
	{
		if (ft_strncmp(envp[i], name, len_name) == 0
			&& envp[i][len_name] == '=')
		{
			new_var = create_env_string(name, value);
			if (!new_var)
				return (1);
			free(envp[i]);
			envp[i] = new_var;
			return (0);
		}
		i++;
	}
	return (update_env_array(envp, count, name, value));
}

char	*my_getenv(char **envp, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = strlen(name);
	while (envp[i])
	{
		if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}

int	is_valid_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}
