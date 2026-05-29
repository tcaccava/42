/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 16:28:07 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 16:28:08 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_arrow(t_token **mat)
{
	size_t	count;

	if (mat == NULL)
		return (0);
	count = count_tokens(mat) - 1;
	if (mat[count]->sub_type & (IN | OUT | APPEND))
		return (1);
	else if (mat[count]->sub_type & HEREDOC)
		return (2);
	return (0);
}

static t_token	*select_creation(char *line, size_t *i, size_t *j,
		t_token **tokens)
{
	t_token	*token;

	token = NULL;
	if (!is_arrow(tokens))
		token = create_cmd(line, i, j, CMD);
	else if (is_arrow(tokens) == 1)
		token = create_cmd(line, i, j, FILENAME);
	else if (is_arrow(tokens) == 2)
		token = create_cmd(line, i, j, LIMITER);
	return (token);
}

static void	process_non_special_chars(char *line, size_t *i, size_t *j,
		t_token ***tokens)
{
	while (line[*j] && !special_char(line[*j]))
	{
		*tokens = add_token(*tokens, select_creation(line, i, j, *tokens));
		jump_spaces(line, i, j);
	}
}

static void	process_special_chars(char *line, size_t *i, size_t *j,
		t_token ***tokens)
{
	while (line[*j] && special_char(line[*j]))
	{
		*tokens = add_token(*tokens, create_special(line, i, j));
		jump_spaces(line, i, j);
	}
}

int	tokenizer(char *line, t_token ***tokens)
{
	size_t	i;
	size_t	j;
	t_token	*final;

	i = 0;
	j = 0;
	if (line[0] == '\0')
		return (1);
	while (line[j])
	{
		jump_spaces(line, &i, &j);
		process_non_special_chars(line, &i, &j, tokens);
		process_special_chars(line, &i, &j, tokens);
	}
	final = create_token(ft_strdup("newline"), END, END);
	*tokens = add_token(*tokens, final);
	return (0);
}
