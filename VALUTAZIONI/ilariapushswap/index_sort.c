/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:44:52 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/29 03:41:35 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*Function that copies the values ​​in the linked list
a into a dynamic array of integers (arr).
Calculates the size of the list (size) using ft_lstsize(a).
Allocates memory for the array of integers (arr)
that will hold the values ​​of the nodes in the list.
Iterates through each node in the list, copying
the value of the node (temp->content) into the array (arr).
Returns the array containing the values ​​in the list.*/
static int	*copy_list_to_array(t_list *a)
{
	int		size;
	int		*arr;
	int		i;
	t_list	*temp;

	size = ft_lstsize(a);
	arr = malloc(sizeof(int) * size);
	if (!arr)
		return (NULL);
	temp = a;
	i = 0;
	while (temp)
	{
		arr[i] = temp->content;
		i++;
		temp = temp->next;
	}
	return (arr);
}

/*Iterates through the array with two nested loops:
The first loop (i) scans the array from the beginning to 
the second-to-last position.
The second loop (j) compares each element with the following ones 
and,if it finds a larger value, it swaps them.*/
static void	bubble_sort_array(int *arr, int size)
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (arr[i] > arr[j])
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

/*function assigns an index to each node in the linked list a,
based on the order of the values ​​in the sorted array sorted.
Iterates through the nodes in the list one by one (temp).
For each node, it looks up the node's value in the sorted array sorted.
When it finds a matching value,
it assigns the index attribute of the node to the index 
of that value in the sorted array.
Repeats this process for each node in the list.*/
static void	assign_node_index(t_list *a, int *sorted, int size)
{
	int		i;
	int		found;
	t_list	*temp;

	temp = a;
	while (temp)
	{
		i = 0;
		found = 0;
		while (i < size && !found)
		{
			if (temp->content == sorted[i])
			{
				temp->index = i;
				found = 1;
			}
			i++;
		}
		temp = temp->next;
	}
}

/*Combines the other functions to assign indices to the nodes of list a. 
Tests whether list a is empty, in which case it immediately exits.
Computes the size of list a.
Creates an array that copies the values ​​of the list 
by calling copy_list_to_array.
Sorts the array using bubble_sort_array.
Assigns indices to the nodes of the list by calling assign_node_index.
Frees the memory of the sorted array once the process is complete.*/
void	assign_index(t_list *a)
{
	int		*arr;
	int		size;

	if (!a)
		return ;
	size = ft_lstsize(a);
	arr = copy_list_to_array(a);
	if (!arr)
		return ;
	bubble_sort_array(arr, size);
	assign_node_index(a, arr, size);
	free(arr);
}

int	is_sorted(t_list *stack)
{
	while (stack && stack->next)
	{
		if (stack->content > stack->next->content)
			return (0);
		stack = stack->next;
	}
	return (1);
}
