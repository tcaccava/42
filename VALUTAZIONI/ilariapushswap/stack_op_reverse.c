/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_op_reverse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:37:02 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/19 13:08:37 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rra(t_list **stack_a)
{
	t_list	*prev;
	t_list	*curr;

	if (!stack_a || !*stack_a || !((*stack_a)->next))
		return ;
	prev = NULL;
	curr = *stack_a;
	while (curr->next)
	{
		prev = curr;
		curr = curr->next;
	}
	prev->next = NULL;
	curr->next = *stack_a;
	*stack_a = curr;
	write(1, "rra\n", 4);
}

void	rrb(t_list **stack_b)
{
	t_list	*prev;
	t_list	*curr;

	if (!stack_b || !*stack_b || !((*stack_b)->next))
		return ;
	prev = NULL;
	curr = *stack_b;
	while (curr->next)
	{
		prev = curr;
		curr = curr->next;
	}
	prev->next = NULL;
	curr->next = *stack_b;
	*stack_b = curr;
	write(1, "rrb\n", 4);
}

void	rrr(t_list **stack_a, t_list **stack_b)
{
	int	performed;

	performed = 0;
	if (stack_a && *stack_a && (*stack_a)->next)
	{
		rra(stack_a);
		performed = 1;
	}
	if (stack_b && *stack_b && (*stack_b)->next)
	{
		rrb(stack_b);
		performed = 1;
	}
	if (performed)
		write(1, "rrr\n", 4);
}
