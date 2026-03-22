/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 14:44:58 by abkhefif          #+#    #+#             */
/*   Updated: 2025/04/27 06:32:15 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tree(t_tree_node *node)
{
	if (!node)
		return ;
	free_tree(node->left);
	free_tree(node->right);
	if (node->type == NODE_COMMAND && node->cmd)
		free_commands(node->cmd);
	free(node);
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = temp;
	}
}

void	free_command_resources(t_command *cmd)
{
	int	i;

	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	if (cmd->heredoc_limiter)
		free(cmd->heredoc_limiter);
	if (cmd->subshell_cmds)
		free_commands(cmd->subshell_cmds);
}

void	free_commands(t_command *cmd)
{
	t_command	*temp;

	while (cmd)
	{
		temp = cmd->next;
		free_command_resources(cmd);
		free(cmd);
		cmd = temp;
	}
}

void	free_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
