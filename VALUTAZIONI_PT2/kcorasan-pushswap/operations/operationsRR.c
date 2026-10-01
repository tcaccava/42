/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operationsRR.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcorasan <kcorasan@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 22:37:55 by sara              #+#    #+#             */
/*   Updated: 2026/09/14 17:17:59 by kcorasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <unistd.h>

static void	rev_rotate(t_stack *s)
{
	t_node	*tmp;
	t_node	*prev;

	if (s->size < 2)
		return ;
	prev = s->top;
	while (prev->next->next != NULL)
		prev = prev->next;
	tmp = prev->next;
	prev->next = NULL;
	tmp->next = s->top;
	s->top = tmp;
}

void	rra(t_stack *a, t_count *count)
{
	count->rra++;
	count->total++;
	rev_rotate(a);
	if (!count->is_bench)
		write(1, "rra\n", 4);
}

void	rrb(t_stack *b, t_count *count)
{
	count->rrb++;
	count->total++;
	rev_rotate(b);
	if (!count->is_bench)
		write(1, "rrb\n", 4);
}

void	rrr(t_stack *a, t_stack *b, t_count *count)
{
	count->rrr++;
	count->total++;
	rev_rotate(a);
	rev_rotate(b);
	if (!count->is_bench)
		write(1, "rrr\n", 4);
}
