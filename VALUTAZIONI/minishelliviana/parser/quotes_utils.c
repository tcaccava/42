/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:02:47 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 16:02:48 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	line_left_shift(char *line, size_t j)
{
	while (line[j])
	{
		line[j] = line[j + 1];
		j++;
	}
}

void	remove_quotes(char *line)
{
	size_t	j;
	char	to_find;

	j = 0;
	to_find = 0;
	while (line[j])
	{
		if (line[j] == '"' || line[j] == '\'')
		{
			to_find = line[j];
			line_left_shift(line, j);
			while (line[j] && line[j] != to_find)
				j++;
			if (line[j])
				line_left_shift(line, j);
		}
		else
			j++;
	}
}

t_token	*check_quotes(t_token *token, t_token **res)
{
	size_t	j;
	char	*line;
	char	to_find;

	j = 0;
	line = (char *)token->content;
	to_find = 0;
	while (line[j])
	{
		if (line[j] == '"' || line[j] == '\'')
		{
			to_find = line[j++];
			while (line[j] && line[j] != to_find)
				j++;
			if (!line[j])
			{
				*res = token;
				break ;
			}
		}
		j++;
	}
	return (*res);
}
