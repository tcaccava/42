/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_indexing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmanno <fmanno@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:24:50 by fmanno            #+#    #+#             */
/*   Updated: 2025/08/26 10:29:13 by fmanno           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../push_swap.h"

void	assign_index(t_list **stack)
{
	t_list	*current;
	int		index;

	index = 0;
	current = get_next_min(stack);
	while (current)
	{
		current->index = index++;
		current = get_next_min(stack);
	}
}

int	get_min(t_list **stack, int exclude)
{
	t_list	*current;
	int		min;

	min = 2147483647;
	current = *stack;
	while (current)
	{
		if (current->index != exclude && current->index < min)
			min = current->index;
		current = current->next;
	}
	return (min);
}

t_list	*get_next_min(t_list **stack)
{
	t_list	*curr;
	t_list	*min;
	int		found_min;

	min = NULL;
	found_min = 0;
	curr = *stack;
	while (curr)
	{
		if ((curr->index == -1) && (!found_min || curr->value < min->value))
		{
			min = curr;
			found_min = 1;
		}
		curr = curr->next;
	}
	return (min);
}

int	get_distance_to_index(t_list **stack, int index)
{
	t_list	*current;
	int		distance;

	distance = 0;
	current = *stack;
	while (current)
	{
		if (current->index == index)
			break ;
		distance++;
		current = current->next;
	}
	return (distance);
}

void	move_to_top(t_list **stack, int distance)
{
	t_list	*current;
	int		rra_count;

	if (distance == 0)
		return ;
	current = *stack;
	rra_count = ft_lstsize(current) - distance;
	if (distance <= (ft_lstsize(current) / 2))
	{
		while (distance-- > 0)
			ra(stack);
	}
	else
	{
		while (rra_count-- > 0)
			rra(stack);
	}
}
