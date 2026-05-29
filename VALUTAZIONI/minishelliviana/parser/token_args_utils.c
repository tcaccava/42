/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_args_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtodaro <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:31:00 by rtodaro           #+#    #+#             */
/*   Updated: 2025/04/18 17:31:01 by rtodaro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_args(t_token **tokens, size_t i)
{
	size_t	count;

	count = 0;
	while (tokens[++i] && tokens[i]->type == CMD)
		count++;
	return (count);
}

void	set_args(t_token **args, t_token **tokens, size_t *i)
{
	size_t	j;

	(*i)++;
	j = 0;
	while (tokens[*i] && tokens[*i]->type == CMD)
	{
		args[j] = tokens[*i];
		(*i)++;
		j++;
	}
	args[j] = NULL;
}

int	assign_args(t_token **tokens)
{
	size_t	i;
	size_t	args;
	t_token	*curr;

	i = 0;
	curr = NULL;
	while (tokens[i])
	{
		if (tokens[i]->type == CMD)
		{
			curr = tokens[i];
			args = count_args(tokens, i);
			if (args != 0)
			{
				curr->args = (t_token **)ft_calloc(args + 1, sizeof(t_token *));
				set_args(curr->args, tokens, &i);
			}
			else
				i++;
		}
		else
			i++;
	}
	return (0);
}
/*
void	print_args(t_token **tokens)
{
	size_t  i;
	size_t  j;

	i = 0;
	j = 0;
	printf("PRINTING ARGS!\n\n");
	while (tokens[i])
	{
		if (tokens[i]->type == CMD && tokens[i]->args)
		{
			printf("command %s:\n", (char *)tokens[i]->content);
			while (tokens[i] && tokens[i]->args[j])
			{
				printf("arg %lu: %s\n", j,
						(char *)tokens[i]->args[j]->content);
				j++;
			}
			printf("\n");
			j = 0;
			while (tokens[i] && tokens[i]->type == CMD)
				i++;
		}
		else
			i++;
	}
	printf("ARGS PRINTED!\n");
}*/
