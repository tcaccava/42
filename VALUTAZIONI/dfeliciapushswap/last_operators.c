/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_operators.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 16:42:11 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/14 01:18:25 by dfelicia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

void	rrr(t_list **stacka, t_list **stackb, t_ops *counter)
{
	counter->rrr++;
	rra(stacka, 0, counter);
	rrb(stackb, 0, counter);
	ft_printf("rrr\n");
}

void	twostack(t_list **stack, t_ops *counter)
{
	if ((int)(long)(*stack)->content < ((int)(long)(*stack)->next->content))
		return ;
	else
		sa(stack, 1, counter);
	return ;
}

void	threestack(t_list **stack, t_ops *counter)
{
	int	a;
	int	b;
	int	c;

	a = (int)(long)(*stack)->content;
	b = (int)(long)(*stack)->next->content;
	c = (int)(long)(*stack)->next->next->content;
	if (a < b && b > c && a < c)
	{
		rra(stack, 1, counter);
		sa(stack, 1, counter);
	}
	else if (a > b && b < c && a < c)
		sa(stack, 1, counter);
	else if (a < b && b > c && a > c)
		rra(stack, 1, counter);
	else if (a > b && b < c && a > c)
		ra(stack, 1, counter);
	else if (a > b && b > c && a > c)
	{
		sa(stack, 1, counter);
		rra(stack, 1, counter);
	}
}

int	sortchecker(t_list *stack)
{
	while (stack->next != NULL)
	{
		if ((int)(long)stack->content > (int)(long)stack->next->content)
			return (1);
		stack = stack->next;
	}
	return (0);
}

int	find_min(t_list	*stack)
{
	int	min;
	int	minpointer;
	int	i;

	i = 0;
	minpointer = 0;
	min = (int)(long)stack->content;
	while (stack)
	{
		if (min > (int)(long)stack->content)
		{
			min = (int)(long)stack->content;
			minpointer = i;
		}
		stack = stack->next;
		i++;
	}
	return (minpointer);
}
