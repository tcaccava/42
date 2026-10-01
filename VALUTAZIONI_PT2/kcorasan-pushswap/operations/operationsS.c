/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operationsS.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcorasan <kcorasan@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 00:18:27 by sara              #+#    #+#             */
/*   Updated: 2026/09/14 17:18:23 by kcorasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <unistd.h>

static void	swap(t_stack *s)
{
	int	tmp;

	if (s->size < 2)
		return ;
	tmp = s->top->value;
	s->top->value = s->top->next->value;
	s->top->next->value = tmp;
}

void	sa(t_stack *a, t_count *count)
{
	count->sa++;
	count->total++;
	swap(a);
	if (!count->is_bench)
		write(1, "sa\n", 3);
}

void	sb(t_stack *b, t_count *count)
{
	count->sb++;
	count->total++;
	swap(b);
	if (!count->is_bench)
		write(1, "sb\n", 3);
}

void	ss(t_stack *a, t_stack *b, t_count *count)
{
	count->ss++;
	count->total++;
	swap(a);
	swap(b);
	if (!count->is_bench)
		write(1, "ss\n", 3);
}
