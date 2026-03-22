/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:33:03 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/27 10:01:24 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redirection(char *input, int *i, t_token **head, t_token **current)
{
	if (input[*i] == 60)
		return (handle_less_operator(input, i, head, current));
	else if (input[*i] == 62)
		return (handle_greater_operator(input, i, head, current));
	return (0);
}

int	execute_command_node(t_tree_node *root, char **env)
{
	if (root->cmd->is_subshell && root->cmd->subshell_node)
		return (execute_tree(root->cmd->subshell_node, env));
	else
		return (execute_command(root, env));
}
