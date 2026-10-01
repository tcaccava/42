/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adaptive.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcorasan <kcorasan@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 20:45:23 by sara              #+#    #+#             */
/*   Updated: 2026/09/14 17:15:27 by kcorasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	adaptive_sort(t_stack *a, t_stack *b, t_count *count)
{
	if (count->disorder < 0.2)
	{
		simple_sort(a, b, count);
		count->strategy = "simple";
		count->complexity = "O(n^2)";
	}
	else if (count->disorder < 0.5)
	{
		medium_sort(a, b, count);
		count->strategy = "medium";
		count->complexity = "O(n√n)";
	}
	else
	{
		complex_sort(a, b, count);
		count->strategy = "complex";
		count->complexity = "O(n log n)";
	}
}

void	select_strategy(char **argv, t_stack *a, t_stack *b, t_count *count)
{
	if (has_flag(argv, "--bench"))
		count->is_bench = 0;
	else
		count->is_bench = 0;
	if (has_flag(argv, "--simple"))
	{
		simple_sort(a, b, count);
		count->strategy = "simple";
		count->complexity = "O(n^2)";
	}
	else if (has_flag(argv, "--medium"))
	{
		medium_sort(a, b, count);
		count->strategy = "medium";
		count->complexity = "O(n√n)";
	}
	else if (has_flag(argv, "--complex"))
	{
		complex_sort(a, b, count);
		count->strategy = "complex";
		count->complexity = "O(n log n)";
	}
	else
		adaptive_sort(a, b, count);
}
