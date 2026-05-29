/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_special.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:28:25 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 16:28:26 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_and(char *line, size_t *i, size_t *j)
{
	if (line[*j + 1] == '&')
	{
		*j += 2;
		*i = *j;
		return (create_token(ft_strdup("&&"), DELIMETER, AND));
	}
	(*j)++;
	*i = *j;
	return (create_token(ft_strdup("&"), DELIMETER, AND));
}

static t_token	*handle_or(char *line, size_t *i, size_t *j)
{
	if (line[*j + 1] == '|')
	{
		*j += 2;
		*i = *j;
		return (create_token(ft_strdup("||"), DELIMETER, OR));
	}
	(*j)++;
	*i = *j;
	return (create_token(ft_strdup("|"), REDIRECT, PIPE));
}

static t_token	*handle_less(char *line, size_t *i, size_t *j)
{
	if (line[*j + 1] == '<')
	{
		*j += 2;
		*i = *j;
		return (create_token(ft_strdup("<<"), REDIRECT, HEREDOC));
	}
	(*j)++;
	*i = *j;
	return (create_token(ft_strdup("<"), REDIRECT, IN));
}

static t_token	*handle_greater(char *line, size_t *i, size_t *j)
{
	if (line[*j + 1] == '>')
	{
		*j += 2;
		*i = *j;
		return (create_token(ft_strdup(">>"), REDIRECT, APPEND));
	}
	(*j)++;
	*i = *j;
	return (create_token(ft_strdup(">"), REDIRECT, OUT));
}

t_token	*create_special(char *line, size_t *i, size_t *j)
{
	if (line[*j] == '&')
		return (handle_and(line, i, j));
	if (line[*j] == '|')
		return (handle_or(line, i, j));
	if (line[*j] == '<')
		return (handle_less(line, i, j));
	if (line[*j] == '>')
		return (handle_greater(line, i, j));
	if (line[*j] == '(')
	{
		(*j)++;
		*i = *j;
		return (create_token(ft_strdup("("), OPEN, OPEN));
	}
	if (line[*j] == ')')
	{
		(*j)++;
		*i = *j;
		return (create_token(ft_strdup(")"), CLOSE, CLOSE));
	}
	return (NULL);
}
