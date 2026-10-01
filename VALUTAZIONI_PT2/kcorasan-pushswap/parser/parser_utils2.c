/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 12:19:52 by sara              #+#    #+#             */
/*   Updated: 2026/07/27 12:31:12 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	count_split(char **argv, int i)
{
	char	**split;
	int		j;
	int		count;

	split = ft_split(argv[i], ' ');
	if (!split)
		return (0);
	j = 0;
	count = 0;
	while (split[j])
	{
		count++;
		j++;
	}
	free_split(split);
	return (count);
}
