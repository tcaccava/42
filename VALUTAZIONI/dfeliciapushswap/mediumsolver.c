/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mediumsolver.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <mde-matt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 14:59:50 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/21 13:06:16 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

void	stackfree(t_list *stack)
{
	t_list	*next;

	while (stack)
	{
		next = stack->next;
		free(stack);
		stack = next;
	}
}

t_list	*stackdup(t_list *stack)
{
	t_list	*dup;
	t_list	*cpy;

	dup = NULL;
	while (stack)
	{
		cpy = malloc(sizeof(t_list));
		if (!cpy)
		{
			stackfree(dup);
			return (NULL);
		}
		cpy->content = stack->content;
		cpy->next = NULL;
		ft_lstadd_back(&dup, cpy);
		stack = stack->next;
	}
	return (dup);
}

int	groupfinder(t_list **stacka, t_list *original, int group)
{
	int		pos;
	t_list	*tmp;

	tmp = *stacka;
	pos = 0;
	while (tmp && groupassigner((int)(long)tmp->content, original)
		!= group)
	{
		tmp = tmp->next;
		pos++;
	}
	return (pos);
}

void	mediumsolver(t_list **stacka, t_list **stackb, t_ops *counter)
{
	int		group;
	int		population;
	int		i;
	t_list	*original;

	group = 0;
	original = stackdup(*stacka);
	while (group < groupdivider(ft_lstsize(original)))
	{
		i = 0;
		population = group_population(original, group);
		while (i < population)
		{
			minraiser(stacka, groupfinder(stacka, original, group), counter);
			chunk_pb(stacka, stackb, i, counter);
			i++;
		}
		group++;
	}
	while (*stackb)
		pa(stacka, stackb, counter);
	stackfree(original);
}
