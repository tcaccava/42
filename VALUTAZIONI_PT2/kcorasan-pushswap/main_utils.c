/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 16:22:45 by sara              #+#    #+#             */
/*   Updated: 2026/09/13 16:32:30 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	init_sort(t_stack *a, t_stack *b, t_count *count, char **numbers)
{
	ft_memset(count, 0, sizeof(t_count));
	init_stacks(a, b);
	if (!build_stack(numbers, a))
		return (0);
	assign_rank(a);
	return (1);
}
