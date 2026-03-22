/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_3_2_5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 01:35:24 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/19 20:19:14 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_3(t_stack **stack)
{
	int	a;
	int	b;
	int	c;

	if (!stack || !(*stack) || !(*stack)->next || !(*stack)->next->next)
		return ;
	a = (*stack)->value;
	b = (*stack)->next->value;
	c = (*stack)->next->next->value;
	if (a > b && b < c && a < c)
		sa(*stack);
	else if (a > b && b > c)
	{
		sa(*stack);
		rra(stack);
	}
	else if (a > b && b < c && a > c)
		ra(stack);
	else if (a < b && b > c && a > c)
		rra(stack);
	else if (a < b && b > c && a < c)
	{
		sa(*stack);
		ra(stack);
	}
}

int	find_min(t_stack *stack)
{
	int	min;

	if (!stack)
		write_error();
	min = stack->value;
	while (stack)
	{
		if (stack->value < min)
			min = stack->value;
		stack = stack->next;
	}
	return (min);
}

void	move_to_top(t_stack **stack, int value)
{
	t_stack	*current;
	int		position;
	int		stack_size;

	position = 0;
	current = *stack;
	stack_size = get_stack_size(*stack);
	while (current)
	{
		if (current->value == value)
			break ;
		position++;
		current = current->next;
	}
	if (position <= stack_size / 2)
	{
		while ((*stack)->value != value)
			ra(stack);
	}
	else
	{
		while ((*stack)->value != value)
			rra(stack);
	}
}

int	get_stack_size(t_stack *stack)
{
	int	size;

	size = 0;
	while (stack)
	{
		size++;
		stack = stack->next;
	}
	return (size);
}

void	sort_5(t_stack **a, t_stack **b)
{
	int	min1;
	int	min2;

	min1 = find_min(*a);
	move_to_top(a, min1);
	pb(a, b);
	min2 = find_min(*a);
	move_to_top(a, min2);
	pb(a, b);
	sort_3(a);
	pa(a, b);
	pa(a, b);
}
