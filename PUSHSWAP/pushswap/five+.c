/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   five+.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:52:56 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 12:27:21 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	rotate_both_up(t_stack **a, t_stack **b, int *cost_a, int *cost_b)
{
	while (*cost_a > 0 && *cost_b > 0)
	{
		rr(a, b);
		(*cost_a)--;
		(*cost_b)--;
	}
}

static void	rotate_both_down(t_stack **a, t_stack **b, int *cost_a, int *cost_b)
{
	while (*cost_a < 0 && *cost_b < 0)
	{
		rrr(a, b);
		(*cost_a)++;
		(*cost_b)++;
	}
}

void	execute_rotation(t_stack **a, t_stack **b, int cost_a, int cost_b)
{
	rotate_both_up(a, b, &cost_a, &cost_b);
	rotate_both_down(a, b, &cost_a, &cost_b);
	while (cost_a > 0)
	{
		ra(a);
		cost_a--;
	}
	while (cost_a < 0)
	{
		rra(a);
		cost_a++;
	}
	while (cost_b > 0)
	{
		rb(b);
		cost_b--;
	}
	while (cost_b < 0)
	{
		rrb(b);
		cost_b++;
	}
}

void	execute_cheapest_move(t_stack **a, t_stack **b)
{
	t_stack	*cheapest;
	int		cost_a;
	int		cost_b;

	if (!a || !*a || !b || !*b)
		return ;
	set_position(a);
	set_position(b);
	set_target_positions(a, b);
	calculate_cost(a, b);
	cheapest = find_cheapest_move(*b);
	if (!cheapest)
		return ;
	cost_a = cheapest->cost_a;
	cost_b = cheapest->cost_b;
	execute_rotation(a, b, cost_a, cost_b);
	pa(a, b);
}

void	rotate_to_min(t_stack **a)
{
	int	size;
	int	min_pos;
	int	rotations_needed;

	if (!a || !*a)
		return ;
	set_position(a);
	size = get_stack_size(*a);
	min_pos = find_min_position(*a);
	if (min_pos == 0)
		return ;
	if (min_pos <= size / 2)
	{
		while (min_pos-- > 0)
			ra(a);
	}
	else
	{
		rotations_needed = size - min_pos;
		while (rotations_needed > 0)
		{
			rra(a);
			rotations_needed--;
		}
	}
}
