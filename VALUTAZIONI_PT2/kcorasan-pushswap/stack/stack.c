/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 01:23:18 by sara              #+#    #+#             */
/*   Updated: 2026/07/28 05:11:04 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <stdio.h>

// crea un nuovo nodo
t_node	*new_node(int num)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->value = num;
	node->next = NULL;
	return (node);
}

// aggiunde un nodo in cima alla lista
void	stack_add_front(t_node **top, t_node *new)
{
	if (!top || !new)
		return ;
	new->next = *top;
	*top = new;
}

// costruisce lo stack a degli arg. si parte dall'ultimo
// cosi il primo finiscce su
int	build_stack(char **numbers, t_stack *a)
{
	int		i;
	t_node	*node;

	i = 0;
	while (numbers[i])
		i++;
	i--;
	while (i >= 0)
	{
		node = new_node(ft_atoi(numbers[i]));
		if (!node)
			return (0);
		stack_add_front(&a->top, node);
		a->size++;
		i--;
	}
	return (1);
}

// libera tutti i nodi
void	free_stack(t_stack *s)
{
	t_node	*current;
	t_node	*next;

	current = s->top;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	s->top = NULL;
	s->size = 0;
}
// scorre la lista e ctrl che ogni value sia minore del

int	is_sorted(t_stack *a)
{
	t_node	*current;

	current = a->top;
	while (current->next != NULL)
	{
		if (current->value > current->next->value)
			return (0);
		current = current->next;
	}
	return (1);
}
