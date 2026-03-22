/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   five+2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 08:23:51 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 08:27:47 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	set_position(t_stack **stack)
{
	t_stack	*current;
	int		i;

	if (!stack || !*stack)
		return ;
	current = *stack;
	i = 0;
	while (current)
	{
		current->pos = i;
		current = current->next;
		i++;
	}
}

int	find_min_position(t_stack *stack)
{
	t_stack	*current;
	int		min_value;
	int		min_pos;
	int		current_pos;

	if (!stack)
		return (0);
	current = stack;
	min_value = current->value;
	min_pos = 0;
	current_pos = 0;
	while (current)
	{
		if (current->value < min_value)
		{
			min_value = current->value;
			min_pos = current_pos;
		}
		current = current->next;
		current_pos++;
	}
	return (min_pos);
}

int	find_max_position(t_stack *stack)
{
	t_stack	*current;
	int		max_value;
	int		max_pos;
	int		current_pos;

	if (!stack)
		return (0);
	current = stack;
	max_value = current->value;
	max_pos = 0;
	current_pos = 0;
	while (current)
	{
		if (current->value > max_value)
		{
			max_value = current->value;
			max_pos = current_pos;
		}
		current = current->next;
		current_pos++;
	}
	return (max_pos);
}

void	find_min_value_and_pos(t_stack *stack, int *min_value, int *min_pos)
{
	t_stack	*current;
	int		pos;

	current = stack;
	*min_value = INT_MAX;
	*min_pos = 0;
	pos = 0;
	while (current)
	{
		if (current->value < *min_value)
		{
			*min_value = current->value;
			*min_pos = pos;
		}
		current = current->next;
		pos++;
	}
}

int	find_target_position(t_stack *a, int b_value)
{
	t_stack	*current;
	int		target_pos;
	int		closest_larger;
	int		min_value;
	int		min_pos;

	find_min_value_and_pos(a, &min_value, &min_pos);
	current = a;
	closest_larger = INT_MAX;
	target_pos = 0;
	while (current)
	{
		if (current->value > b_value && current->value < closest_larger)
		{
			closest_larger = current->value;
			target_pos = current->pos;
		}
		current = current->next;
	}
	if (closest_larger == INT_MAX)
		return (min_pos);
	return (target_pos);
}
