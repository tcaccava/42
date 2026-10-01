/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   medium.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 04:22:03 by sara              #+#    #+#             */
/*   Updated: 2026/09/12 23:04:40 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

// scorre i nodi, e assegna un numero ad ogni elem, che sarà la sua pos.
void	assign_rank(t_stack *a)
{
	t_node	*all;
	t_node	*ctrl;
	int		rank;

	all = a->top;
	while (all)
	{
		ctrl = a->top;
		rank = 0;
		while (ctrl)
		{
			if (ctrl->value < all->value)
				rank++;
			ctrl = ctrl->next;
		}
		all->rank = rank;
		all = all->next;
	}
}

// calcolo la radice quadrata per intero
int	calcola_radq(int size)
{
	int	i;

	i = 1;
	while (i * i <= size)
		i++;
	return (i - 1);
}

int	get_chunk_size(int size)
{
	int	chunks;

	chunks = calcola_radq(size);
	return (size / chunks);
}

int	find_chunk_pos(t_stack *a, int chunk, int chunk_size)
{
	t_node	*ptn_a;
	int		pos;

	ptn_a = a->top;
	pos = 0;
	while (ptn_a)
	{
		if (ptn_a->rank >= chunk * chunk_size && ptn_a->rank < (chunk + 1)
			* chunk_size)
			return (pos);
		ptn_a = ptn_a->next;
		pos++;
	}
	return (-1);
}

void	medium_sort(t_stack *a, t_stack *b, t_count *count)
{
	int	chunk;
	int	chunk_size;
	int	pos;
	int	fw;

	pos = 0;
	chunk = 0;
	chunk_size = get_chunk_size(a->size);
	while (a->size > 0)
	{
		pos = find_chunk_pos(a, chunk, chunk_size);
		if (pos == -1)
		{
			chunk++;
		}
		else
		{
			fw = is_index_near(a, pos);
			move_stack(a, pos, fw, count);
			pb(a, b, count);
		}
	}
	finish_sort_medium(a, b, count);
}
