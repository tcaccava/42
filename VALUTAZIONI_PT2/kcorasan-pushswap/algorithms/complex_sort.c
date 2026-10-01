/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcorasan <kcorasan@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 12:28:33 by kcorasan          #+#    #+#             */
/*   Updated: 2026/09/14 17:20:11 by kcorasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	get_bit(int n, int bit)
{
	return ((n >> bit) & 1);
}

int	get_max_bit(t_stack *stack)
{
	int	max_bits;

	max_bits = 0;
	while ((stack->size >> max_bits) != 0)
	{
		max_bits++;
	}
	return (max_bits);
}

void	init(t_stack *a, int *max_bits, int *size, int *bit)
{
	*max_bits = get_max_bit(a);
	*size = a->size;
	*bit = 0;
}

void	complex_sort(t_stack *a, t_stack *b, t_count *count)
{
	t_pair	*rank_array;
	int		bit;
	int		i;
	int		max_bits;
	int		size;

	rank_array = complex_create_ranks(a);
	init(a, &max_bits, &size, &bit);
	while (bit < max_bits)
	{
		i = 0;
		while (i < size)
		{
			if (get_bit(a->top->value, bit) == 0)
				pb(a, b, count);
			else
				ra(a, count);
			i++;
		}
		while (b->size > 0)
			pa(a, b, count);
		bit++;
	}
	complex_free_ranks(rank_array, a);
}
