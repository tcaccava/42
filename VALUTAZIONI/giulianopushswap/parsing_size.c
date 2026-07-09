/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 18:19:48 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/07 10:58:52 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	size_3(t_list **a, t_count *count)
{
	while (!is_ordered(*a))
	{
		if ((*a)->nb > (*a)->next->nb)
		{
			if (is_greatest_number((*a)->nb, *a))
				rotate(a, 'a', count);
			else
				swap(a, 'a', count);
		}
		else
			reverse_rotate(a, 'a', count);
	}
}

static void	push_lesser_in_b(
		t_list **a,
		t_list **b,
		int lst_size,
		t_count *count)
{
	int	nb;
	int	distance;

	nb = lesser_number(*a);
	distance = target_distance(nb, *a);
	while ((*a)->nb != nb)
	{
		if (lst_size / 2 <= distance)
			reverse_rotate(a, 'a', count);
		else
			rotate(a, 'a', count);
	}
	push(b, a, 'b', count);
}

static void	size_4(t_list **a, t_list **b, int lst_size, t_count *count)
{
	push_lesser_in_b(a, b, lst_size, count);
	size_3(a, count);
	push(a, b, 'a', count);
}

static void	size_5(t_list **a, t_list **b, int lst_size, t_count *count)
{
	push_lesser_in_b(a, b, lst_size, count);
	push_lesser_in_b(a, b, lst_size, count);
	size_3(a, count);
	push(a, b, 'a', count);
	push(a, b, 'a', count);
}

int	parsing_size(t_list **a, t_list **b, int lst_size, t_count *count)
{
	if (lst_size < 6 && lst_size > 1)
	{
		if (lst_size == 2 && (*a)->nb > (*a)->next->nb)
			swap(a, 'a', count);
		else if (lst_size == 3)
			size_3(a, count);
		else if (lst_size == 4)
			size_4(a, b, lst_size, count);
		else if (lst_size == 5)
			size_5(a, b, lst_size, count);
		return (1);
	}
	return (0);
}
