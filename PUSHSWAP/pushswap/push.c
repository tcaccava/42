/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:56:24 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 18:00:32 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	pa(t_stack **a, t_stack **b)
{
	t_stack	*temp;

	if (*b)
	{
		temp = *b;
		*b = (*b)->next;
		temp->next = *a;
		*a = temp;
		write(1, "pa\n", 3);
	}
}

void	pb(t_stack **a, t_stack **b)
{
	t_stack	*temp;

	if (*a)
	{
		temp = *a;
		*a = (*a)->next;
		temp->next = *b;
		*b = temp;
		write(1, "pb\n", 3);
	}
}

void	print_stack(t_stack *stack)
{
	t_stack	*current;

	current = stack;
	while (current)
	{
		printf("%d ", current->value);
		current = current->next;
	}
}

void	silent_pb(t_stack **a, t_stack **b)
{
	t_stack	*temp;

	if (*a)
	{
		temp = *a;
		*a = (*a)->next;
		temp->next = *b;
		*b = temp;
	}
}
