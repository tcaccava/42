/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 23:47:15 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 17:58:24 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_stack	*create_node(int value)
{
	t_stack	*new;

	new = malloc(sizeof(t_stack));
	if (!new)
		write_error();
	new->value = value;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	add_to_stack(t_stack **stack, int value)
{
	t_stack	*new;
	t_stack	*current;

	new = create_node(value);
	if (!*stack)
	{
		*stack = new;
		return ;
	}
	current = *stack;
	while (current->next)
		current = current->next;
	current->next = new;
	new->prev = current;
}

void	silent_ss(t_stack *a, t_stack *b)
{
	int	temp;

	if (a && a->next)
	{
		temp = a->value;
		a->value = a->next->value;
		a->next->value = temp;
	}
	if (b && b->next)
	{
		temp = b->value;
		b->value = b->next->value;
		b->next->value = temp;
	}
}
