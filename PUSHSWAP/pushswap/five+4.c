/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   five+4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 12:10:43 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 12:11:46 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_large(t_stack **a, t_stack **b)
{
	if (!a || !*a)
		return ;
	assign_index(a);
	optimized_initial_push(a, b);
	sort_3(a);
	while (*b)
	{
		set_position(a);
		set_position(b);
		set_target_positions(a, b);
		calculate_cost(a, b);
		execute_cheapest_move(a, b);
	}
	set_position(a);
	rotate_to_min(a);
}

void	fill_array_from_stack(t_stack *stack, int *arr, int size)
{
	t_stack	*current;
	int		i;

	if (stack == 0)
		return ;
	current = stack;
	i = 0;
	while (current != 0 && i < size)
	{
		arr[i] = current->value;
		i++;
		current = current->next;
	}
}

void	bubble_sort_array(int *arr, int size)
{
	int	i;
	int	j;
	int	temp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (arr[i] > arr[j])
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
			j++;
		}
		i++;
	}
}

void	assign_index_to_stack(t_stack *stack, int *arr, int size)
{
	t_stack	*current;
	int		i;

	current = stack;
	while (current != 0)
	{
		i = 0;
		while (i < size)
		{
			if (current->value == arr[i])
			{
				current->index = i;
				break ;
			}
			i++;
		}
		current = current->next;
	}
}

void	assign_index(t_stack **a)
{
	t_stack	*stack;
	int		size;
	int		*arr;

	if (a == 0 || *a == 0)
		return ;
	stack = *a;
	size = get_stack_size(stack);
	arr = malloc(sizeof(int) * size);
	if (arr == 0)
		return ;
	fill_array_from_stack(stack, arr, size);
	bubble_sort_array(arr, size);
	assign_index_to_stack(stack, arr, size);
	free(arr);
}
