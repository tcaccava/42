/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pspaccas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 23:33:39 by pspaccas          #+#    #+#             */
/*   Updated: 2025/06/23 23:34:28 by pspaccas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/push_swap.h"

void	check_stack(t_stack_node **a, t_stack_node **b)
{
	if (stack_len(*a) == 2)
		sa(a, false);
	else if (stack_len(*a) == 3)
		sort_three(a);
	else
		sort_stacks(a, b);
}

int	main(int argc, char **argv)
{
	t_stack_node	*a;
	t_stack_node	*b;
	bool			is_split;

	a = NULL;
	b = NULL;
	is_split = false;
	if (argc == 1 || (argc == 2 && !argv[1][0]))
		return (1);
	else if (argc == 2)
	{
		is_split = true;
		argv = split(argv[1], ' ');
	}
	init_stack_a(&a, argv + 1);
	if (is_split)
		free_split(argv);
	if (!stack_sorted(a))
	{
		check_stack(&a, &b);
	}
	free_stack(&a);
	return (0);
}
