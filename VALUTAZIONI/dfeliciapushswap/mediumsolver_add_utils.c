/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mediumsolver_add_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 18:43:05 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/14 17:26:18 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

int	group_population(t_list *stack, int c)
{
	int		population;
	t_list	*stackcpy;

	population = 0;
	stackcpy = stack;
	while (stack)
	{
		if (groupassigner((int)(long)stack->content, stackcpy) == c)
			population++;
		stack = stack->next;
	}
	return (population);
}

int	count_bigger(t_list *stackb, int limit, int value)
{
	t_list	*cpy;
	int		k;
	int		i;

	cpy = stackb;
	k = 0;
	i = 0;
	while (cpy && i < limit)
	{
		if ((int)(long)cpy->content > value)
			k++;
		cpy = cpy->next;
		i++;
	}
	return (k);
}

void	chunk_pb(t_list **stacka, t_list **stackb, int limit, t_ops *counter)
{
	int	value;
	int	k;
	int	bot;

	value = (int)(long)(*stacka)->content;
	k = count_bigger(*stackb, limit, value);
	bot = k;
	while (k)
	{
		rb(stackb, 1, counter);
		k--;
	}
	pb(stacka, stackb, counter);
	while (bot)
	{
		rrb (stackb, 1, counter);
		bot--;
	}
}
