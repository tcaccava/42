/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 19:59:39 by tcaccava          #+#    #+#             */
/*   Updated: 2025/03/05 19:59:52 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_stack	*a;
	t_stack	*b;
	int		stack_size;

	if (argc < 2)
		return (0);
	a = validate_input(argc, argv);
	if (get_stack_size(a) <= 1 || is_sorted(a))
		return (0);
	b = NULL;
	stack_size = get_stack_size(a);
	if (stack_size == 2 && a->value > a->next->value)
		sa(a);
	else if (stack_size == 3)
		sort_3(&a);
	else if (stack_size <= 5)
		sort_5(&a, &b);
	else
		sort_large(&a, &b);
	free_stack(&a);
	free_stack(&b);
	return (0);
}
