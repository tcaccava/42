/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operationsP.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcorasan <kcorasan@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 04:49:40 by sara              #+#    #+#             */
/*   Updated: 2026/09/14 17:16:35 by kcorasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <unistd.h>

static void	push(t_stack *src, t_stack *dest)
{
	t_node	*tmp;

	if (src->size < 1)
		return ;
	tmp = src->top;
	src->top = src->top->next;
	tmp->next = dest->top;
	dest->top = tmp;
	src->size--;
	dest->size++;
}

void	pa(t_stack *a, t_stack *b, t_count *count)
{
	count->pa++;
	count->total++;
	push(b, a);
	if (!count->is_bench)
		write(1, "pa\n", 3);
}

void	pb(t_stack *a, t_stack *b, t_count *count)
{
	count->pb++;
	count->total++;
	push(a, b);
	if (!count->is_bench)
		write(1, "pb\n", 3);
}
