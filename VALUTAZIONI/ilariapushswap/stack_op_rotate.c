/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_op_rotate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:29:10 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/19 13:23:55 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ra(t_list **stack_a)
{
	t_list	*first;
	t_list	*temp;

	if (!stack_a || !*stack_a || !((*stack_a)->next))
		return ;
	first = *stack_a;
	*stack_a = first->next;
	first->next = NULL;
	temp = *stack_a;
	while (temp->next)
		temp = temp->next;
	temp->next = first;
	write(1, "ra\n", 3);
}

void	rb(t_list **stack_b)
{
	t_list	*first;
	t_list	*temp;

	if (!stack_b || !*stack_b || !((*stack_b)->next))
		return ;
	first = *stack_b;
	*stack_b = first->next;
	first->next = NULL;
	temp = *stack_b;
	while (temp->next)
		temp = temp->next;
	temp->next = first;
	write(1, "rb\n", 3);
}

static int	rotate_stack(t_list **stack)
{
	t_list	*first;
	t_list	*temp;

	if (!stack || !*stack || !(*stack)->next)
		return (0);
	first = *stack;
	*stack = first->next;
	first->next = NULL;
	temp = *stack;
	while (temp->next)
		temp = temp->next;
	temp->next = first;
	return (1);
}

void	rr(t_list **stack_a, t_list **stack_b)
{
	int	performed;

	performed = 0;
	if (rotate_stack(stack_a))
		performed = 1;
	if (rotate_stack(stack_b))
		performed = 1;
	if (performed)
		write(1, "rr\n", 3);
}
