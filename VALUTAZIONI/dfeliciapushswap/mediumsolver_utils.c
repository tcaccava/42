/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mediumsolver_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 18:31:57 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/14 17:25:37 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

int	groupdivider(int n)
{
	int	groups;

	groups = 1;
	while (groups * groups < n)
		groups++;
	return (groups);
}

int	find_max(t_list *stack)
{
	int	max;
	int	maxpointer;
	int	i;

	i = 0;
	maxpointer = 0;
	max = (int)(long)stack->content;
	while (stack)
	{
		if (max < (int)(long)stack->content)
		{
			max = (int)(long)stack->content;
			maxpointer = i;
		}
		stack = stack->next;
		i++;
	}
	return (maxpointer);
}

int	min_value(t_list *stack)
{
	int	i;
	int	pos;

	pos = find_min(stack);
	i = 0;
	while (i < pos)
	{
		stack = stack->next;
		i++;
	}
	return ((int)(long)stack->content);
}

int	max_value(t_list *stack)
{
	int	i;
	int	pos;

	i = 0;
	pos = find_max(stack);
	while (i < pos)
	{
		stack = stack->next;
		i++;
	}
	return ((int)(long)stack->content);
}

int	groupassigner(int value, t_list *stack)
{
	int	range;
	int	group;

	range = ((max_value(stack) - min_value(stack) + 1)
			/ groupdivider(ft_lstsize(stack))) + 1;
	group = (value - min_value(stack)) / range;
	return (group);
}
