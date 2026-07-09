/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   radix_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 17:27:25 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/06 17:57:04 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	push_back_in_a_radix(t_list **a, t_list **b, t_count *count)
{
	while (*b)
		push(a, b, 'a', count);
}

static void	push_in_b_radix(t_list **a, t_list **b, t_count *count, int i)
{
	int	size;
	int	j;

	size = ft_lstsize(*a);
	j = 0;
	while (j < size)
	{
		if ((((*a)->idx >> i) & 1) == 0)
			push(b, a, 'b', count);
		else
			rotate(a, 'a', count);
		j++;
	}
}

static int	bit_len(int nb)
{
	int	bit_len;
	int	nb_moved;

	bit_len = 0;
	nb_moved = nb;
	while (nb_moved > 0)
	{
		bit_len++;
		nb_moved = nb_moved >> 1;
	}
	return (bit_len);
}

void	radix_sort(t_list **a, t_list **b, t_count *count)
{
	int	i;
	int	len;

	if (!count->adaptive_called)
		count->strategy = ft_strdup("O(n log n)");
	i = 0;
	len = bit_len(greatest_index(*a));
	while (i < len)
	{
		push_in_b_radix(a, b, count, i);
		push_back_in_a_radix(a, b, count);
		i++;
	}
}
