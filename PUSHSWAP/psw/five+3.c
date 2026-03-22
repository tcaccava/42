/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   five+3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:25:26 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 12:11:59 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	set_target_positions(t_stack **a, t_stack **b)
{
	t_stack	*current_b;

	if (!a || !*a || !b || !*b)
		return ;
	current_b = *b;
	while (current_b)
	{
		current_b->target_pos = find_target_position(*a, current_b->value);
		current_b = current_b->next;
	}
}

void	calculate_cost(t_stack **a, t_stack **b)
{
	t_stack	*current_b;
	int		size_a;
	int		size_b;

	if (!a || !*a || !b || !*b)
		return ;
	size_a = get_stack_size(*a);
	size_b = get_stack_size(*b);
	current_b = *b;
	while (current_b)
	{
		if (current_b->pos <= size_b / 2)
			current_b->cost_b = current_b->pos;
		else
			current_b->cost_b = -(size_b - current_b->pos);
		if (current_b->target_pos <= size_a / 2)
			current_b->cost_a = current_b->target_pos;
		else
			current_b->cost_a = -(size_a - current_b->target_pos);
		current_b = current_b->next;
	}
}

t_stack	*find_cheapest_move(t_stack *b)
{
	t_stack	*current;
	t_stack	*cheapest;
	int		cheapest_cost;
	int		current_cost;

	if (!b)
		return (NULL);
	current = b;
	cheapest = b;
	cheapest_cost = abs(b->cost_a) + abs(b->cost_b);
	while (current)
	{
		current_cost = abs(current->cost_a) + abs(current->cost_b);
		if (current_cost < cheapest_cost)
		{
			cheapest_cost = current_cost;
			cheapest = current;
		}
		current = current->next;
	}
	return (cheapest);
}

int	abs(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

void	optimized_initial_push(t_stack **a, t_stack **b)
{
	int	total;
	int	chunk_size;
	int	current_chunk;

	total = get_stack_size(*a);
	chunk_size = total / 4;
	current_chunk = chunk_size;
	while (get_stack_size(*a) > 3)
	{
		if ((*a)->index <= current_chunk)
			pb(a, b);
		else
			ra(a);
		if (get_stack_size(*b) == current_chunk)
			current_chunk += chunk_size;
	}
}
