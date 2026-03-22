/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:09:36 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/26 14:44:22 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_entries(char **envp, int count)
{
	int	i;

	i = count;
	while (i > 0)
	{
		i--;
		free(envp[i]);
	}
	free(envp);
}

char	**copy_env(char **env)
{
	int		count;
	char	**envp;
	int		i;

	count = 0;
	while (env[count])
		count++;
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		envp[i] = strdup(env[i]);
		if (!envp[i])
		{
			free_env_entries(envp, i);
			return (NULL);
		}
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

int	ft_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (1);
	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
