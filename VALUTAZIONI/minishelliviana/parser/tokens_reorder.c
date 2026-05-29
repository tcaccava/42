/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_reorder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:30:48 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 17:30:49 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redirection_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->sub_type
		& (IN | OUT | HEREDOC | APPEND | FILENAME | LIMITER));
}

static long	shift_tokens_left(t_token **tokens, long src_index, long dest_index)
{
	t_token	*temp;

	temp = tokens[src_index];
	while (src_index > dest_index)
	{
		tokens[src_index] = tokens[src_index - 1];
		src_index--;
	}
	tokens[src_index] = temp;
	return (dest_index);
}

static long	find_next_non_cmd_token(t_token **tokens, long start_index)
{
	while (tokens[start_index] && tokens[start_index]->type == CMD)
		start_index++;
	return (start_index);
}

void	reorder_tokens(t_token **tokens)
{
	long	current_pos;
	long	next_pos;

	current_pos = 0;
	next_pos = 0;
	while (tokens[next_pos])
	{
		current_pos = next_pos;
		next_pos = find_next_non_cmd_token(tokens, current_pos);
		if (tokens[next_pos] && is_redirection_token(tokens[next_pos]))
		{
			next_pos = shift_tokens_left(tokens, next_pos, current_pos);
			current_pos++;
			next_pos = current_pos;
		}
		else if (tokens[next_pos])
		{
			next_pos++;
			current_pos = next_pos;
		}
	}
}

void	assign_index(t_token **tokens)
{
	size_t	i;
	size_t	index;
	t_type	temp_sub;

	i = 0;
	index = 0;
	tokens[i]->index = index;
	temp_sub = tokens[i]->sub_type;
	i++;
	while (tokens[i])
	{
		if (temp_sub & (CMD) && tokens[i]->sub_type & (CMD))
			tokens[i]->index = index;
		else
		{
			tokens[i]->index = i;
			index = i;
		}
		temp_sub = tokens[i]->sub_type;
		i++;
	}
}
