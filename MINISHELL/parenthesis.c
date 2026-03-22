/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 20:49:52 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/26 19:38:25 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_lparen(char *input, int *i, t_token **head, t_token **current)
{
	t_token	*new_token;

	(void)input;
	new_token = create_token(TOKEN_LP, "(");
	if (!new_token)
		return (0);
	(*i)++;
	add_token(head, current, new_token);
	return (1);
}

int	handle_rparen(char *input, int *i, t_token **head, t_token **current)
{
	t_token	*new_token;

	(void)input;
	new_token = create_token(TOKEN_RP, ")");
	if (!new_token)
		return (0);
	(*i)++;
	add_token(head, current, new_token);
	return (1);
}

static int	skip_parentheses(t_token **tokens)
{
	int	paren_count;

	paren_count = 1;
	while (*tokens && paren_count > 0)
	{
		if ((*tokens)->type == TOKEN_LP)
			paren_count++;
		else if ((*tokens)->type == TOKEN_RP)
			paren_count--;
		if (paren_count > 0)
			*tokens = (*tokens)->next;
	}
	return (paren_count);
}

t_command	*parse_parentheses(t_token **tokens, char **envp, int exit_status)
{
	t_command	*cmd_list;
	t_token		*start;
	t_command	*subshell_cmd;
	int			paren_count;

	*tokens = (*tokens)->next;
	start = *tokens;
	paren_count = skip_parentheses(tokens);
	if (!*tokens || paren_count > 0)
		return (NULL);
	subshell_cmd = init_cmd();
	if (!subshell_cmd)
		return (NULL);
	subshell_cmd->is_subshell = 1;
	cmd_list = parse_range(start, *tokens, envp, exit_status);
	subshell_cmd->subshell_cmds = cmd_list;
	*tokens = (*tokens)->next;
	return (subshell_cmd);
}
