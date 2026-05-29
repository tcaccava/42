/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hybrid_sorting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:13:47 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/28 18:17:51 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

//Calcola il numero massimo di bit necessari
static int	get_max_bits(int max_index)
{
	int	max_bits;

	max_bits = 0;
	while ((max_index >> max_bits) != 0)
		max_bits++;
	return (max_bits);
}

//Gestisce un singolo passaggio del radix sort
static void	sort_pass(t_list **a, t_list **b, int bit)
{
	int	size;
	int	j;
	int	pb_count;

	size = ft_lstsize(*a);
	j = 0;
	pb_count = 0;
	while (j < size)
	{
		if (((*a)->index >> bit) & 1)
			ra(a);
		else
		{
			pb(a, b);
			pb_count++;
		}
		j++;
	}
	while (*b)
		pa(a, b);
}

void	radix_sort(t_list **a, t_list **b)
{
	int	size;
	int	max_index;
	int	max_bits;
	int	i;
	int	sorted;

	size = ft_lstsize(*a);
	assign_index(*a);
	if (size <= 5)
	{
		sort_small(a, b, size);
		return ;
	}
	max_index = size - 1;
	max_bits = get_max_bits(max_index);
	i = 0;
	sorted = 0;
	while (i < max_bits && !sorted)
	{
		sort_pass(a, b, i);
		sorted = is_sorted(*a);
		i++;
	}
}
