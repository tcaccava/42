/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <abkhefif@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:09:42 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/26 14:09:43 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	display_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("declare -x %s\n", envp[i]);
		i++;
	}
}

static int	export_variable(char *arg, char **envp)
{
	char	*equals_pos;
	int		name_len;
	char	*var_name;
	char	*var_value;

	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
	{
		name_len = equals_pos - arg;
		var_name = malloc(sizeof(char) * (name_len + 1));
		if (!var_name)
			return (1);
		ft_strncpy(var_name, arg, name_len);
		var_name[name_len] = '\0';
		var_value = equals_pos + 1;
		set_env_var(envp, var_name, var_value);
		free(var_name);
	}
	return (0);
}

int	ft_export(t_tree_node *root, char **envp)
{
	int	i;

	if (!root->cmd->args[1])
	{
		display_envp(envp);
		return (0);
	}
	i = 1;
	while (root->cmd->args[i])
	{
		if (export_variable(root->cmd->args[i], envp))
			return (1);
		i++;
	}
	return (0);
}
