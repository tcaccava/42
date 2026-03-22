/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 11:51:06 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/26 19:02:58 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*find_and_or_cmd(t_command *cmd_list)
{
	t_command	*last_and_or;

	last_and_or = NULL;
	while (cmd_list)
	{
		if (cmd_list->next_separator == SEP_AND
			|| cmd_list->next_separator == SEP_OR)
			last_and_or = cmd_list;
		cmd_list = cmd_list->next;
	}
	return (last_and_or);
}

static t_command	*find_pipe_cmd(t_command *cmd_list)
{
	while (cmd_list)
	{
		if (cmd_list->next_separator == SEP_PIPE)
			return (cmd_list);
		cmd_list = cmd_list->next;
	}
	return (NULL);
}

static t_tree_node	*handle_and_or_split(t_command *cmd_list, t_command *split)
{
	t_tree_node	*left;
	t_tree_node	*right;
	t_node_type	type;
	t_command	*right_cmds;

	if (split->next_separator == SEP_AND)
		type = NODE_AND;
	else
		type = NODE_OR;
	right_cmds = split->next;
	split->next = NULL;
	split->next_separator = SEP_END;
	left = build_tree(cmd_list);
	right = build_tree(right_cmds);
	return (create_operator_node(type, left, right));
}

static t_tree_node	*handle_pipe_split(t_command *cmd_list, t_command *split)
{
	t_tree_node	*left;
	t_tree_node	*right;
	t_command	*right_cmds;

	right_cmds = split->next;
	split->next = NULL;
	split->next_separator = SEP_END;
	left = build_tree(cmd_list);
	right = build_tree(right_cmds);
	return (create_operator_node(NODE_PIPE, left, right));
}

t_tree_node	*build_tree(t_command *cmd_list)
{
	t_command	*split_point;
	t_tree_node	*subshell_tree;

	if (!cmd_list)
		return (NULL);
	split_point = find_and_or_cmd(cmd_list);
	if (split_point)
		return (handle_and_or_split(cmd_list, split_point));
	split_point = find_pipe_cmd(cmd_list);
	if (split_point)
		return (handle_pipe_split(cmd_list, split_point));
	if (cmd_list->is_subshell)
	{
		subshell_tree = build_tree(cmd_list->subshell_cmds);
		cmd_list->subshell_node = subshell_tree;
		return (create_cmd_node(cmd_list));
	}
	return (create_cmd_node(cmd_list));
}
