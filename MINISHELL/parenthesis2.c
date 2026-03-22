/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:50:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/26 19:38:34 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_token(t_token **current, t_command *new_cmd, char **envp,
		int exit_status)
{
	char	*expanded;
	t_token	*token_ptr;

	if ((*current)->type == TOKEN_WORD)
	{
		if ((*current)->quote_type != '\'')
			expanded = expand_variables((*current)->value, envp, exit_status);
		else
			expanded = ft_strdup((*current)->value);
		add_to_args(new_cmd, expanded);
		free(expanded);
	}
	else if ((*current)->type == IN || (*current)->type == OUT
		|| (*current)->type == APPEND || (*current)->type == HERE)
	{
		token_ptr = *current;
		handle_redirection_token(&token_ptr, new_cmd, envp, exit_status);
		*current = token_ptr;
	}
}

static void	set_separator_and_advance(t_token **current, t_command *new_cmd,
		t_token *end)
{
	if (*current && *current != end)
	{
		if ((*current)->type == TOKEN_PIPE)
			new_cmd->next_separator = SEP_PIPE;
		else if ((*current)->type == TOKEN_AND)
			new_cmd->next_separator = SEP_AND;
		else if ((*current)->type == TOKEN_OR)
			new_cmd->next_separator = SEP_OR;
		*current = (*current)->next;
	}
	else
		new_cmd->next_separator = SEP_END;
}

t_command	*parse_range(t_token *start, t_token *end, char **envp,
		int exit_status)
{
	t_command	*cmd_list;
	t_command	*new_cmd;
	t_token		*current;

	cmd_list = NULL;
	current = start;
	while (current && current != end)
	{
		new_cmd = init_cmd();
		if (!new_cmd)
			return (NULL);
		while (current && current != end && current->type != TOKEN_PIPE
			&& current->type != TOKEN_AND && current->type != TOKEN_OR)
		{
			process_token(&current, new_cmd, envp, exit_status);
			if (current)
				current = current->next;
		}
		set_separator_and_advance(&current, new_cmd, end);
		add_cmd_to_list(&cmd_list, new_cmd);
	}
	return (cmd_list);
}
