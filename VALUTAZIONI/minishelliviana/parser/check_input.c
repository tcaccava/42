/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:56:35 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 16:02:00 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_invalid_start(t_token **tokens)
{
	if (tokens[0]->sub_type & (AND | OR | PIPE | CLOSE))
		return (1);
	return (0);
}

static int	is_background_amp(t_token *token)
{
	if (token && token->sub_type == AND)
		if (!ft_strcmp((char *)token->content, "&"))
			return (1);
	return (0);
}

static int	handle_quotes(t_token *token, t_token **res)
{
	t_token	*match;

	match = check_quotes(token, res);
	if (match)
	{
		token->quotes = 1;
		return (1);
	}
	return (0);
}

static int	handle_brackets(t_token *token, size_t *brackets, int *unmatched)
{
	if (token->type == OPEN)
		(*brackets)++;
	else if (token->type == CLOSE && *brackets == 0)
	{
		*unmatched = 1;
		return (1);
	}
	else if (token->type == CLOSE)
		(*brackets)--;
	return (0);
}

t_token	*check_args(t_token **tokens)
{
	t_token	*res;
	size_t	i;
	size_t	brackets;
	int		unmatched;

	res = NULL;
	i = -1;
	brackets = 0;
	unmatched = 0;
	if (is_invalid_start(tokens))
		return (tokens[0]);
	while (tokens[++i])
	{
		if (is_background_amp(tokens[i]))
			return (tokens[i]);
		if (check_next(tokens, i, &res))
			break ;
		if (handle_quotes(tokens[i], &res))
			return (tokens[i]);
		if (handle_brackets(tokens[i], &brackets, &unmatched))
			return (tokens[i]);
	}
	return (res);
}
