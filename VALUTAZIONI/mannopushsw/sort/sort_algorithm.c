/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_algorithm.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmanno <fmanno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:24:29 by fmanno            #+#    #+#             */
/*   Updated: 2025/08/26 16:51:30 by fmanno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

int	get_bit_length(t_list **stack)
{
	t_list	*current;
	int		max;
	int		max_bits;

	current = *stack;
	max = current->index;
	max_bits = 0;
	while (current)
	{
		if (current->index > max)
			max = current->index;
		current = current->next;
	}
	while ((max >> max_bits) != 0)
		max_bits++;
	return (max_bits);
}

void	radix_sort(t_list **stack_a, t_list **stack_b)
{
	t_list	*current_a;
	int		curr_bit;
	int		i;
	int		size;
	int		max_bits;

	curr_bit = 0;
	current_a = *stack_a;
	size = ft_lstsize(current_a);
	max_bits = get_bit_length(stack_a);
	while (curr_bit < max_bits)
	{
		i = 0;
		while (i++ < size)
		{
			current_a = *stack_a;
			if (((current_a->index >> curr_bit) & 1) == 1)
				ra(stack_a);
			else
				pb(stack_a, stack_b);
		}
		while (ft_lstsize(*stack_b) != 0)
			pa(stack_a, stack_b);
		curr_bit++;
	}
}
