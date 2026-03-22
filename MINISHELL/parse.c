/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:32:06 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/27 02:20:50 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_words_token(t_token **tokens, t_command *cmd, char **envp,
		int exit_status)
{
	char	*expanded;

	if ((*tokens)->quote_type != '\'')
		expanded = expand_variables((*tokens)->value, envp, exit_status);
	else
		expanded = ft_strdup((*tokens)->value);
	add_to_args(cmd, expanded);
	free(expanded);
}

void	handle_parentheses(t_token **tokens, t_command *cmd, char **envp,
		int exit_status)
{
	t_command	*subshell_cmd;

	subshell_cmd = parse_parentheses(tokens, envp, exit_status);
	if (subshell_cmd)
	{
		cmd->subshell_cmds = subshell_cmd;
		cmd->is_subshell = 1;
	}
}

void	process_tokens(t_token **tokens, t_command *cmd, char **envp,
		int exit_status)
{
	while (*tokens && (*tokens)->type != TOKEN_AND
		&& (*tokens)->type != TOKEN_OR && (*tokens)->type != TOKEN_EOF
		&& (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_RP)
	{
		if ((*tokens)->type == TOKEN_LP)
		{
			handle_parentheses(tokens, cmd, envp, exit_status);
			continue ;
		}
		if ((*tokens)->type == TOKEN_WORD)
		{
			handle_words_token(tokens, cmd, envp, exit_status);
		}
		else
		{
			handle_redirection_token(tokens, cmd, envp, exit_status);
			continue ;
		}
		*tokens = (*tokens)->next;
	}
}

void	handle_redirection_token(t_token **tokens, t_command *cmd, char **envp,
		int status)
{
	char	*expanded;
	int		type;

	type = (*tokens)->type;
	if ((*tokens)->next && (*tokens)->next->type == TOKEN_WORD)
	{
		*tokens = (*tokens)->next;
		if ((*tokens)->quote_type != '\'')
			expanded = expand_variables((*tokens)->value, envp, status);
		else
			expanded = ft_strdup((*tokens)->value);
		if (type == IN)
			cmd->input_file = expanded;
		else if (type == OUT)
			cmd->output_file = expanded;
		else if (type == APPEND)
		{
			cmd->output_file = expanded;
			cmd->append = 1;
		}
		else if (type == HERE)
			cmd->heredoc_limiter = expanded;
	}
	*tokens = (*tokens)->next;
}

t_command	*parse(t_token *tokens, char **envp, int exit_status)
{
	t_command	*cmd_list;
	t_command	*new_cmd;

	cmd_list = NULL;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		new_cmd = init_cmd();
		if (!new_cmd)
			return (NULL);
		process_tokens(&tokens, new_cmd, envp, exit_status);
		if (!tokens)
			return (NULL);
		set_separator(new_cmd, tokens);
		add_cmd_to_list(&cmd_list, new_cmd);
		if (tokens && (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AND
				|| tokens->type == TOKEN_OR))
			tokens = tokens->next;
	}
	apply_wild_cmd_list(cmd_list);
	return (cmd_list);
}
