/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_operation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:10:55 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/29 02:27:56 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sa(t_list **stack_a)
{
	if (!stack_a || !*stack_a || !((*stack_a)->next))
		return ;
	swappy(stack_a);
	write(1, "sa\n", 3);
}

void	sb(t_list **stack_b)
{
	if (!stack_b || !*stack_b || !((*stack_b)->next))
		return ;
	swappy(stack_b);
	write(1, "sb\n", 3);
}

void	ss(t_list **stack_a, t_list **stack_b)
{
	int	swapped;

	swapped = 0;
	if (stack_a && *stack_a && (*stack_a)->next)
	{
		swappy(stack_a);
		swapped = 1;
	}
	if (stack_b && *stack_b && (*stack_b)->next)
	{
		swappy(stack_b);
		swapped = 1;
	}
	if (swapped)
		write(1, "ss\n", 3);
}

void	pa(t_list **stack_a, t_list **stack_b)
{
	t_list	*node;

	if (!stack_b || !*stack_b)
		return ;
	node = *stack_b;
	*stack_b = node->next;
	node->next = *stack_a;
	*stack_a = node;
	write(1, "pa\n", 3);
}

void	pb(t_list **stack_a, t_list **stack_b)
{
	t_list	*node;

	if (!stack_a || !*stack_a)
		return ;
	node = *stack_a;
	*stack_a = node->next;
	node->next = *stack_b;
	*stack_b = node;
	write(1, "pb\n", 3);
}
