/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   little_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:25:50 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/29 03:31:25 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*function that sorts small stacks by size,
calling other specific functions*/
void	sort_small(t_list **a, t_list **b, int size)
{
	if (size == 2 && (*a)->index > (*a)->next->index)
		sa(a);
	else if (size == 3)
		sort_three(a);
	else if (size == 4 || size == 5)
		sort_four(a, b);
}

void	sort_three(t_list **a)
{
	int	numb1;
	int	numb2;
	int	numb3;

	numb1 = (*a)->index;
	numb2 = (*a)->next->index;
	numb3 = (*a)->next->next->index;
	if (numb1 > numb2 && numb2 < numb3 && numb1 < numb3)
		sa(a);
	else if (numb1 > numb2 && numb2 > numb3)
	{
		sa(a);
		rra(a);
	}
	else if (numb1 > numb2 && numb2 < numb3 && numb1 > numb3)
		ra(a);
	else if (numb1 < numb2 && numb2 > numb3 && numb1 < numb3)
	{
		sa(a);
		ra(a);
	}
	else if (numb1 < numb2 && numb2 > numb3 && numb1 > numb3)
		rra(a);
}

void	sort_four(t_list **a, t_list **b)
{
	while (ft_lstsize(*a) > 3)
		move_smallest_b(a, b);
	sort_three(a);
	while (*b)
		pa(a, b);
}

/*Finds the smallest element in stack a and 
returns the smallest node with its position.*/
t_list	*small_node(t_list *a, int *min_pos)
{
	t_list	*min;
	t_list	*list;
	int		i;

	min = a;
	list = a;
	*min_pos = 0;
	i = 0;
	while (list)
	{
		if (list->index < min->index)
		{
			min = list;
			*min_pos = i;
		}
		list = list->next;
		i++;
	}
	return (min);
}

/*
Move the smallest node from stack a to stack b using 
appropriate rotations to place it at the top of the stack.*/
void	move_smallest_b(t_list **a, t_list **b)
{
	int	min_pos;
	int	size;

	small_node(*a, &min_pos);
	size = ft_lstsize(*a);
	if (min_pos <= size / 2)
	{
		while (min_pos--)
			ra(a);
	}
	else
	{
		while (min_pos++ < size)
			rra(a);
	}
	pb(a, b);
}
