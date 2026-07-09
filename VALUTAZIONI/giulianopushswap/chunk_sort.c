/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 14:19:20 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/07 10:55:09 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	nearest_nb(int *intset, t_list **a, int chunk)
{
	int	i;
	int	nearest;
	int	tmp;

	i = 0;
	nearest = INT_MAX;
	while (i < chunk)
	{
		tmp = target_idx_distance(intset[i], *a);
		if (tmp != -1 && tmp < nearest)
			nearest = tmp;
		i++;
	}
	return (nearest);
}

static void	which_half(t_list **a, t_count *count, int *intset, int chunk)
{
	if (nearest_nb(intset, a, chunk) <= ft_lstsize(*a) / 2)
	{
		while (nearest_nb(intset, a, chunk) != 0)
			rotate (a, 'a', count);
	}
	else
	{
		while (nearest_nb(intset, a, chunk) != 0)
			reverse_rotate (a, 'a', count);
	}
}

static int	*fill_intset(int i, const int chunk, int **intset)
{
	int	j;

	free(*intset);
	j = 0;
	*intset = malloc (sizeof(int) * chunk);
	if (!(*intset))
		return (NULL);
	while (j < chunk)
	{
		(*intset)[j] = i;
		i++;
		j++;
	}
	return (*intset);
}

static void	push_in_b(t_list **a, t_list **b, t_count *count)
{
	int	size;
	int	chunk;
	int	*intset;
	int	i;

	i = 0;
	size = ft_lstsize(*a);
	chunk = round_sqrt(size);
	intset = NULL;
	while (i < size)
	{
		if (i % chunk == 0)
			fill_intset(i, chunk, &intset);
		which_half(a, count, intset, chunk);
		push (b, a, 'b', count);
		i++;
	}
	free(intset);
}

void	chunk_sort(t_list **a, t_list **b, t_count *count)
{
	if (!count->adaptive_called)
		count->strategy = ft_strdup("O(n√n)");
	push_in_b(a, b, count);
	push_back_in_a(a, b, count);
}
