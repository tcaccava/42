/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplesolver.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 17:46:44 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/14 01:25:27 by dfelicia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

void	minraiser(t_list **stack, int minpointer, t_ops *counter)
{
	int	size;
	int	i;

	i = 0;
	size = ft_lstsize(*stack);
	if (minpointer < size / 2)
	{
		while (i < minpointer)
		{
			ra(stack, 1, counter);
			i++;
		}
	}
	else
	{
		while (i < (size - minpointer))
		{
			rra(stack, 1, counter);
			i++;
		}
	}
}

void	simplesolver(t_list **stacka, t_list **stackb, t_ops *counter)
{
	while (*stacka)
	{
		minraiser(stacka, find_min(*stacka), counter);
		pb(stacka, stackb, counter);
	}
	while (*stackb)
		pa(stacka, stackb, counter);
}
