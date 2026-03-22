/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:10:55 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/26 14:10:56 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_env_var(char **envp, int index)
{
	int	j;

	free(envp[index]);
	j = index;
	while (envp[j])
	{
		envp[j] = envp[j + 1];
		j++;
	}
}

int	ft_unset(t_tree_node *root, char **envp)
{
	int	i;
	int	len;
	int	argu;

	argu = 1;
	if (!root->cmd->args[1])
		return (0);
	while (root->cmd->args[argu])
	{
		len = ft_strlen(root->cmd->args[argu]);
		i = 0;
		while (envp[i])
		{
			if (ft_strncmp(envp[i], root->cmd->args[argu], len) == 0
				&& envp[i][len] == '=')
			{
				remove_env_var(envp, i);
				break ;
			}
			i++;
		}
		argu++;
	}
	return (0);
}
