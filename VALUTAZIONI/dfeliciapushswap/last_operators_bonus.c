/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_operators_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <mde-matt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/17 17:22:27 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/17 17:48:04 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_checker_bonus.h"

void	rrr(t_list **stacka, t_list **stackb)
{
	rra(stacka);
	rrb(stackb);
}

int	sortchecker(t_list *stack)
{
	if (stack)
	{
		while (stack->next != NULL)
		{
			if ((int)(long)stack->content > (int)(long)stack->next->content)
				return (1);
			stack = stack->next;
		}
	}
	return (0);
}
