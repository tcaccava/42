/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcorasan <kcorasan@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 16:07:54 by kcorasan          #+#    #+#             */
/*   Updated: 2026/09/14 17:20:29 by kcorasan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	from_value_to_rank(t_stack *stack, t_pair *rank_array)
{
	t_node	*node;
	int		i;

	node = stack->top;
	i = 0;
	while (node)
	{
		node->value = rank_array[i].rank;
		i++;
		node = node->next;
	}
}

static int	get_rank(t_stack *stack, int value)
{
	t_node	*node;
	int		rank;

	rank = 0;
	node = stack->top;
	while (node)
	{
		if (node->value < value)
		{
			rank++;
		}
		node = node->next;
	}
	return (rank);
}

t_pair	*complex_create_ranks(t_stack *stack)
{
	t_pair	*rank_array;
	t_node	*node;
	int		i;

	rank_array = malloc(sizeof(t_pair) * stack->size);
	if (!rank_array)
		return (NULL);
	node = stack->top;
	i = 0;
	while (node)
	{
		rank_array[i].value = node->value;
		rank_array[i].rank = get_rank(stack, node->value);
		node = node->next;
		i++;
	}
	from_value_to_rank(stack, rank_array);
	return (rank_array);
}

void	complex_free_ranks(t_pair *rank_array, t_stack *stack)
{
	t_node	*node;
	int		i;

	if (!rank_array)
		return ;
	node = stack->top;
	while (node)
	{
		i = 0;
		while (i < stack->size)
		{
			if (rank_array[i].rank == node->value)
			{
				node->value = rank_array[i].value;
				break ;
			}
			i++;
		}
		node = node->next;
	}
	free(rank_array);
	rank_array = NULL;
}
