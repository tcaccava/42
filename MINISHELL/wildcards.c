/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 07:31:00 by tcaccava          #+#    #+#             */
/*   Updated: 2025/04/27 02:21:39 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	match_wildcard_pattern(char *pattern, char *filename)
{
	if (!*pattern && !*filename)
		return (1);
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		if (!*pattern)
			return (1);
		while (*filename)
		{
			if (match_wildcard_pattern(pattern, filename))
				return (1);
			filename++;
		}
		return (0);
	}
	else if (*pattern == *filename)
		return (match_wildcard_pattern(pattern + 1, filename + 1));
	return (0);
}

void	process_args_with_wildcards(t_command *cmd)
{
	int		count;
	char	**new_args;

	count = count_total_args(cmd);
	new_args = create_new_args_array(cmd, count);
	if (!new_args)
		return ;
	free_string_array(cmd->args);
	cmd->args = new_args;
}

int	contain_wildcard(char *str)
{
	return (ft_strchr(str, '*') != NULL);
}

void	apply_wild_cmd_list(t_command *cmd_list)
{
	t_command	*current;

	current = cmd_list;
	while (current)
	{
		process_args_with_wildcards(current);
		current = current->next;
	}
}
