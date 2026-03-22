/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:00:34 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 17:58:50 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rra(t_stack **a)
{
	t_stack	*last;
	t_stack	*second_last;

	if (*a && ((*a)->next))
	{
		last = *a;
		while (last->next)
			last = last->next;
		second_last = *a;
		while (second_last->next != last)
			second_last = second_last->next;
		second_last->next = NULL;
		last->next = *a;
		*a = last;
		write(1, "rra\n", 4);
	}
}

void	silent_rra(t_stack **a)
{
	t_stack	*last;
	t_stack	*second_last;

	if (*a && ((*a)->next))
	{
		last = *a;
		while (last->next)
			last = last->next;
		second_last = *a;
		while (second_last->next != last)
			second_last = second_last->next;
		second_last->next = NULL;
		last->next = *a;
		*a = last;
	}
}

void	rrb(t_stack **b)
{
	t_stack	*last;
	t_stack	*second_last;

	if (*b && (*b)->next)
	{
		last = *b;
		while (last->next)
			last = last->next;
		second_last = *b;
		while (second_last->next != last)
			second_last = second_last->next;
		second_last->next = NULL;
		last->next = *b;
		*b = last;
		write(1, "rrb\n", 4);
	}
}

void	rrr(t_stack **a, t_stack **b)
{
	t_stack	*last;
	t_stack	*second_last;

	silent_rra(a);
	if (*b && (*b)->next)
	{
		last = *b;
		while (last->next)
			last = last->next;
		second_last = *b;
		while (second_last->next != last)
			second_last = second_last->next;
		second_last->next = NULL;
		last->next = *b;
		*b = last;
	}
	write(1, "rrr\n", 4);
}

void	silent_rrr(t_stack **a, t_stack **b)
{
	t_stack	*last;
	t_stack	*second_last;

	silent_rra(a);
	if (*b && (*b)->next)
	{
		last = *b;
		while (last->next)
			last = last->next;
		second_last = *b;
		while (second_last->next != last)
			second_last = second_last->next;
		second_last->next = NULL;
		last->next = *b;
		*b = last;
	}
}
