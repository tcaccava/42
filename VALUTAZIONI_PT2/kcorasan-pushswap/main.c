/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 22:43:11 by sara              #+#    #+#             */
/*   Updated: 2026/09/13 16:28:55 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include <stdio.h>

// inizializzare lo stack a vuoto
void	init_stack(t_stack *stack)
{
	stack->top = NULL;
	stack->size = 0;
}

static void	free_all(char **numbers, t_stack *a, t_stack *b)
{
	free_split(numbers);
	if (a)
		free_stack(a);
	if (b)
		free_stack(b);
}

// controllino per valid
static int	check_input(char **numbers)
{
	if (!pars_args(numbers) || !not_double(numbers))
	{
		write(2, "Error\n", 6);
		return (0);
	}
	return (1);
}

static void	run_sort(t_stack *a, t_stack *b, t_count *count, char **argv)
{
	count->disorder = disorder(a);
	select_strategy(argv, a, b, count);
	if (has_flag(argv, "--bench"))
		print_bench(count);
}

// qui manca la parte dell'algoritmo dopo is_sorted
int	main(int argc, char **argv)
{
	t_stack	a;
	t_stack	b;
	t_count	count;
	char	**numbers;

	if (argc < 2)
		return (0);
	numbers = build_numbers(argc, argv);
	if (!numbers)
		return (write(2, "Error\n", 6), 1);
	if (!check_input(numbers))
		return (free_all(numbers, NULL, NULL), 1);
	if (!init_sort(&a, &b, &count, numbers))
		return (free_all(numbers, &a, &b), 1);
	if (is_sorted(&a))
	{
		if (has_flag(argv, "--bench"))
			print_bench(&count);
		return (free_all(numbers, &a, &b), 0);
	}
	run_sort(&a, &b, &count, argv);
	free_all(numbers, &a, &b);
	return (0);
}
