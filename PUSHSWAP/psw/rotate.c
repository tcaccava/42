/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 22:00:01 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 17:58:32 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ra(t_stack **a)
{
	t_stack	*temp;
	t_stack	*current;

	if (*a && (*a)->next)
	{
		temp = *a;
		*a = (*a)->next;
		temp->next = NULL;
		current = *a;
		while (current->next)
			current = current->next;
		current->next = temp;
		write(1, "ra\n", 3);
	}
}

void	rb(t_stack **b)
{
	t_stack	*temp;
	t_stack	*current;

	if (*b && (*b)->next)
	{
		temp = *b;
		*b = (*b)->next;
		temp->next = NULL;
		current = *b;
		while (current->next)
			current = current->next;
		current->next = temp;
		write(1, "rb\n", 3);
	}
}

void	rr(t_stack **a, t_stack **b)
{
	t_stack	*temp;
	t_stack	*current;

	if (*a && (*a)->next)
	{
		temp = *a;
		*a = (*a)->next;
		temp->next = NULL;
		current = *a;
		while (current->next)
			current = current->next;
		current->next = temp;
	}
	if (*b && (*b)->next)
	{
		temp = *b;
		*b = (*b)->next;
		temp->next = NULL;
		current = *b;
		while (current->next)
			current = current->next;
		current->next = temp;
	}
	write(1, "rr\n", 3);
}

void	silent_ra(t_stack **a)
{
	t_stack	*temp;
	t_stack	*current;

	if (*a && (*a)->next)
	{
		temp = *a;
		*a = (*a)->next;
		temp->next = NULL;
		current = *a;
		while (current->next)
			current = current->next;
		current->next = temp;
	}
}

void	silent_rr(t_stack **a, t_stack **b)
{
	t_stack	*temp;
	t_stack	*current;

	if (*a && (*a)->next)
	{
		temp = *a;
		*a = (*a)->next;
		temp->next = NULL;
		current = *a;
		while (current->next)
			current = current->next;
		current->next = temp;
	}
	if (*b && (*b)->next)
	{
		temp = *b;
		*b = (*b)->next;
		temp->next = NULL;
		current = *b;
		while (current->next)
			current = current->next;
		current->next = temp;
	}
}
