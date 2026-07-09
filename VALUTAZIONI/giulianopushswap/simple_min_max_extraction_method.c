/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_min_max_extraction_method.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 15:26:51 by gipaglie          #+#    #+#             */
/*   Updated: 2026/07/06 15:32:28 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	push_prev_greatest_nb(
	t_list	**a,
	t_list	**b,
	t_count	*count,
	void (*rotation)(t_list **, char, t_count *))
{
	while ((*b)->nb != prev_greatest_number(*b))
		rotation(b, 'b', count);
	push(a, b, 'a', count);
	return (TRUE);
}

static int	find_target_in_b(
		t_list **a,
		t_list **b,
		t_count *count,
		int prev_moved)
{
	int	greatest_distance;
	int	prev_distance;

	greatest_distance = target_distance(greatest_number(*b), *b);
	prev_distance = target_distance(prev_greatest_number(*b), *b);
	if (greatest_distance >= (ft_lstsize(*b) / 2))
	{
		if ((*b)->next && prev_distance < greatest_distance
			&& prev_distance >= (ft_lstsize(*b) / 2))
			prev_moved = push_prev_greatest_nb(a, b, count, reverse_rotate);
		while (*b && (*b)->nb != greatest_number(*b))
			reverse_rotate(b, 'b', count);
	}
	else
	{
		if ((*b)->next && prev_distance < greatest_distance
			&& prev_distance < (ft_lstsize(*b) / 2))
			prev_moved = push_prev_greatest_nb(a, b, count, rotate);
		while (*b && (*b)->nb != greatest_number(*b))
			rotate(b, 'b', count);
	}
	return (prev_moved);
}

void	push_back_in_a(t_list **a, t_list **b, t_count *count)
{
	int	prev_moved;

	while (*b)
	{
		prev_moved = FALSE;
		if ((*b)->next)
			prev_moved = find_target_in_b(a, b, count, prev_moved);
		push(a, b, 'a', count);
		if (prev_moved && (*a)->next && (*a)->nb > (*a)->next->nb)
			swap(a, 'a', count);
	}
}

static void	push_all_in_b_except_min(t_list **a, t_list **b, t_count *count)
{
	int	min;

	min = lesser_number(*a);
	while ((*a)->next)
	{
		if ((*a)->nb == min)
			rotate(a, 'a', count);
		else
			push(b, a, 'b', count);
	}
}

void	simple_min_max_extraction_method(t_list **a, t_list **b, t_count *count)
{
	int	lst_size;

	if (!count->adaptive_called)
		count->strategy = ft_strdup("O(n²)");
	lst_size = ft_lstsize(*a);
	parsing_size(a, b, lst_size, count);
	push_all_in_b_except_min(a, b, count);
	push_back_in_a(a, b, count);
	reverse_rotate(a, 'a', count);
}
