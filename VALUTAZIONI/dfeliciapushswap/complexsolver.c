/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complexsolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 16:26:21 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/14 01:12:56 by dfelicia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

void	complexsolver(t_list **stacka, t_list **stackb, t_ops *counter)
{
	int	size;
	int	min;
	int	max;
	int	bit;
	int	i;

	size = ft_lstsize(*stacka);
	min = min_value(*stacka);
	max = max_value(*stacka) - min;
	bit = 0;
	while ((1 << bit) <= max)
	{
		i = 0;
		while (i < size)
		{
			if (((int)(long)(*stacka)->content - min) >> bit & 1)
				ra(stacka, 1, counter);
			else
				pb(stacka, stackb, counter);
			i++;
		}
		while (*stackb)
			pa(stacka, stackb, counter);
		bit++;
	}
}
