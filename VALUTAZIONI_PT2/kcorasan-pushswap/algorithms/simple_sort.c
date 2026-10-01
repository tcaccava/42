/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 15:52:35 by kcorasan          #+#    #+#             */
/*   Updated: 2026/09/07 08:36:10 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

// index of min
static int	find_min(t_stack *stack)
{
	t_node	*current;
	int		min;
	int		index;
	int		min_index;

	current = stack->top;
	min = current->value;
	index = 0;
	min_index = 0;
	while (current)
	{
		if (current->value < min)
		{
			min = current->value;
			min_index = index;
		}
		current = current->next;
		index++;
	}
	return (min_index);
}

// check if has to go fw or bw
int	is_index_near(t_stack *stack, int index)
{
	if (index <= stack->size / 2)
		return (1);
	else
		return (0);
}

// move stack
void	move_stack(t_stack *stack, int index, int fw, t_count *count)
{
	if (!fw)
		index = stack->size - index;
	while (index != 0)
	{
		if (fw)
			ra(stack, count);
		else
			rra(stack, count);
		index--;
	}
}

// put all back in a
void	finish_sort(t_stack *a, t_stack *b, t_count *count)
{
	while (b->size > 0)
		pa(a, b, count);
}

// selection_sort (O^2)
void	simple_sort(t_stack *a, t_stack *b, t_count *count)
{
	int	index;
	int	fw;

	if (!a || !b || a->size < 2)
		return ;
	while (a->size > 0)
	{
		index = find_min(a);
		fw = is_index_near(a, index);
		move_stack(a, index, fw, count);
		pb(a, b, count);
	}
	finish_sort(a, b, count);
}
