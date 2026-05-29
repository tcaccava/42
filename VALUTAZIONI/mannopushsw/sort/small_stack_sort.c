/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   small_stack_sort.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmanno <fmanno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:25:02 by fmanno            #+#    #+#             */
/*   Updated: 2025/08/25 19:38:56 by fmanno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	sort_small_stack(t_list **stack_a, t_list **stack_b)
{
	int	size;

	if (is_sorted(stack_a) || ft_lstsize(*stack_a) <= 1)
		return ;
	size = ft_lstsize(*stack_a);
	if (size == 2)
		sa(stack_a);
	else if (size == 3)
		sort_three(stack_a);
	else if (size == 4)
		sort_four(stack_a, stack_b);
	else if (size == 5)
		sort_five(stack_a, stack_b);
}

void	move_min_top(t_list **a, int exclude)
{
	int	min;
	int	d;

	min = get_min(a, exclude);
	d = get_distance_to_index(a, min);
	move_to_top(a, d);
}

void	sort_three(t_list **a)
{
	t_list	*first_node;
	int		min;
	int		next_min;

	if (is_sorted(a))
		return ;
	first_node = *a;
	min = get_min(a, -1);
	next_min = get_min(a, min);
	if (first_node->index == min)
		handle_min_first(a, next_min);
	else if (first_node->index == next_min)
		handle_nextmin_first(a, min);
	else
		handle_max_first(a, min);
}

void	sort_four(t_list **a, t_list **b)
{
	if (is_sorted(a))
		return ;
	move_min_top(a, -1);
	pb(a, b);
	sort_three(a);
	pa(a, b);
}

void	sort_five(t_list **a, t_list **b)
{
	move_min_top(a, -1);
	pb(a, b);
	sort_four(a, b);
	pa(a, b);
}
