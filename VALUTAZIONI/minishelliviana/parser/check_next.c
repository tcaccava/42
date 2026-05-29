/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_next.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 15:56:40 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 15:56:41 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_pair_1(t_token **tokens, size_t i)
{
	if (tokens[i]->sub_type & CMD)
		if (tokens[i + 1]->sub_type & OPEN)
			return (1);
	if (tokens[i]->sub_type & CLOSE)
		if (tokens[i + 1]->sub_type & CMD)
			return (1);
	return (0);
}

static int	is_pair_2(t_token **tokens, size_t i)
{
	if (tokens[i]->sub_type & (AND | OR | PIPE))
		if (tokens[i + 1]->sub_type & (AND | OR | PIPE | CLOSE | END))
			return (1);
	if (tokens[i]->sub_type & OPEN)
		if (tokens[i + 1]->sub_type & (AND | OR | PIPE | CLOSE | END))
			return (1);
	return (0);
}

static int	is_pair_3(t_token **tokens, size_t i)
{
	if (tokens[i]->sub_type & (IN | OUT | APPEND | HEREDOC))
		if (tokens[i + 1]->sub_type & (NEW_LINE | END))
			return (1);
	return (0);
}

static int	should_assign_res(t_token **tokens, size_t i)
{
	int	found;

	found = 0;
	if (is_pair_1(tokens, i))
		found = 1;
	if (is_pair_2(tokens, i))
		found = 1;
	if (is_pair_3(tokens, i))
		found = 1;
	return (found);
}

t_token	*check_next(t_token **tokens, size_t i, t_token **res)
{
	int	should_assign;

	should_assign = should_assign_res(tokens, i);
	if (should_assign)
		*res = tokens[i + 1];
	return (*res);
}
