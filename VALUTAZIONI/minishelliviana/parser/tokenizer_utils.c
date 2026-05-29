/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:25:07 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 16:25:09 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	jump_spaces(char *line, size_t *i, size_t *j)
{
	while (line[*j] == ' ')
		(*j)++;
	*i = *j;
}

int	special_char(char c)
{
	if (c == '|' || c == '&')
		return (1);
	if (c == '<' || c == '>')
		return (2);
	if (c == '(' || c == ')')
		return (3);
	return (0);
}

int	search_special(char *line, size_t i)
{
	while (line[i] && line[i] == ' ')
		i++;
	return (special_char(line[i]));
}

char	*create_str(char *line, size_t i, size_t j)
{
	char	*res;
	size_t	k;

	res = (char *)malloc(j - i + 2);
	k = 0;
	while (i <= j)
	{
		res[k] = line[i];
		i++;
		k++;
	}
	res[k] = '\0';
	return (res);
}
