/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:11:37 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 16:12:16 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_quote_open(char c, int q, char *temp)
{
	if ((c == '"' || c == '\'') && q == 0)
	{
		*temp = c;
		return (1);
	}
	return (0);
}

static int	is_quote_close(char c, int q, char temp)
{
	return (c == temp && q == 1);
}

static int	is_end_char(char c, int q)
{
	if (q != 0)
		return (0);
	return (c == ' ' || c == '\n' || special_char(c));
}

t_token	*create_cmd(char *line, size_t *i, size_t *j, t_type type)
{
	t_token	*cmd;
	char	temp;
	int		q;

	q = 0;
	temp = 0;
	if (line[*j] == '\n')
	{
		cmd = create_token(ft_strdup("newline"), NEW_LINE, NEW_LINE);
		(*j)++;
		return (cmd);
	}
	while (line[*j])
	{
		if (is_quote_open(line[*j], q, &temp))
			q++;
		else if (is_quote_close(line[*j], q, temp))
			q--;
		else if (is_end_char(line[*j], q))
			break ;
		(*j)++;
	}
	cmd = create_token(create_str(line, *i, *j - 1), type, type);
	return (cmd);
}
