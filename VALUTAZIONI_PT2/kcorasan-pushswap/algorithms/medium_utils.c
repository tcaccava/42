/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 19:51:13 by sara              #+#    #+#             */
/*   Updated: 2026/09/13 16:02:19 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	find_max_pos(t_stack *b)
{
	t_node	*current;
	int		max;
	int		index;
	int		max_index;

	current = b->top;
	max = current->value;
	index = 0;
	max_index = 0;
	while (current)
	{
		if (current->value > max)
		{
			max = current->value;
			max_index = index;
		}
		current = current->next;
		index++;
	}
	return (max_index);
}

static void	move_stack_b(t_stack *stack, int index, int fw, t_count *count)
{
	if (!fw)
		index = stack->size - index;
	while (index != 0)
	{
		if (fw)
			rb(stack, count);
		else
			rrb(stack, count);
		index--;
	}
}

void	finish_sort_medium(t_stack *a, t_stack *b, t_count *count)
{
	int	pos;
	int	fw;

	while (b->size > 0)
	{
		pos = find_max_pos(b);
		fw = is_index_near(b, pos);
		move_stack_b(b, pos, fw, count);
		pa(a, b, count);
	}
}
