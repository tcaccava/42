/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:09:39 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/27 09:17:27 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_numeric_arg(char *arg, int *exit_code)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (arg[0] == '-' || arg[0] == '+')
		i = 1;
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9')
		{
			printf("minishell: exit: %s: numeric argument required\n", arg);
			*exit_code = 2;
			return (0);
		}
		len++;
		i++;
	}
	if (len > 19)
	{
		printf("minishell: exit: %s: numeric argument required\n", arg);
		*exit_code = 2;
		return (0);
	}
	return (1);
}

int	ft_exit(t_tree_node *root, char **envp)
{
	int	exit_code;

	exit_code = 0;
	if (root->cmd->args[1])
	{
		if (check_numeric_arg(root->cmd->args[1], &exit_code))
		{
			exit_code = ft_atoi(root->cmd->args[1]);
			if (root->cmd->args[2])
			{
				printf("minishell: exit: too many arguments\n");
				return (1);
			}
		}
	}
	printf("exit\n");
	rl_clear_history();
	free_tree(root);
	free_env(envp);
	exit(exit_code);
	return (exit_code);
}
