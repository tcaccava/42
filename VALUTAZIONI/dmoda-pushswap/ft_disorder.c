/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_disorder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 17:01:05 by dmoda             #+#    #+#             */
/*   Updated: 2026/08/11 15:00:43 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

float	ft_disorder(t_dlist *head, int len)
{
	t_dlist	*j;
	int		mistakes;
	int		total_pairs;
	int		idx_h;
	int		idx_j;

	total_pairs = 0;
	mistakes = 0;
	idx_h = 0;
	while (idx_h++ < len)
	{
		j = head->next;
		idx_j = idx_h - 1;
		while (idx_j++ < len - 1)
		{
			total_pairs++;
			if (head->cont > j->cont)
				mistakes++;
			j = j->next;
		}
		head = head->next;
	}
	if (total_pairs == 0)
		return (0.0);
	return ((float)mistakes / (float)total_pairs);
}
