/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_final_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:46:52 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 17:47:01 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_single_quotes(char *line, long *j)
{
	(*j)++;
	while (line[*j] && line[*j] != '\'')
		(*j)++;
}

static int	iterate_double_quotes(char **line, t_data *data, long *j, long *k)
{
	char	*temp_line;

	while ((*line)[++(*j)] && (*line)[*j] != '"')
	{
		if ((*line)[*j] == '$')
		{
			temp_line = process_string(*line, data, j, k);
			if (!temp_line)
				return (0);
			*line = temp_line;
			return (1);
		}
	}
	return (0);
}

char	*check_export2(char *line, t_data *data)
{
	long	j;
	long	k;
	char	*temp_line;

	j = -1;
	k = 0;
	while (line[++j])
	{
		if (line[j] == '$')
		{
			temp_line = process_string(line, data, &j, &k);
			if (!temp_line)
				continue ;
			line = temp_line;
		}
		else if (line[j] == '"')
		{
			if (iterate_double_quotes(&line, data, &j, &k))
				continue ;
		}
		else if (line[j] == '\'')
			skip_single_quotes(line, &j);
	}
	return (line);
}

int	finalize_tokens(t_token **tokens, t_data *data, t_execute *info)
{
	size_t	i;

	i = 0;
	while (tokens[i])
	{
		if (tokens[i]->sub_type == HEREDOC)
		{
			if (check_heredoc(tokens, &i, data, info) == 256)
			{
				data->exit_status = 130;
				return (256);
			}
		}
		else
			i++;
	}
	return (0);
}
