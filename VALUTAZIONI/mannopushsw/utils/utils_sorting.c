/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_sorting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmanno <fmanno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:24:29 by fmanno            #+#    #+#             */
/*   Updated: 2025/08/25 19:31:02 by fmanno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int	is_sorted(t_list **stack)
{
	t_list	*current;

	if (!stack || !(*stack))
		return (1);
	current = *stack;
	while (current && current->next)
	{
		if (current->value > current->next->value)
			return (0);
		current = current->next;
	}
	return (1);
}

void	handle_min_first(t_list **a, int next_min)
{
	if ((*a)->next->index != next_min)
	{
		ra(a);
		sa(a);
		rra(a);
	}
}

void	handle_nextmin_first(t_list **a, int min)
{
	if ((*a)->next->index == min)
		sa(a);
	else
		rra(a);
}

void	handle_max_first(t_list **a, int min)
{
	if ((*a)->next->index == min)
		ra(a);
	else
	{
		sa(a);
		rra(a);
	}
}
