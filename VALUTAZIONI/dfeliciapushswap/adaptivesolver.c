/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adaptivesolver.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:39:51 by mde-matt          #+#    #+#             */
/*   Updated: 2026/07/14 17:39:20 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

void	adaptivesolver(t_list **stacka, t_list **stackb, t_ops *counter)
{
	int	disorder;
	int	size;

	size = ft_lstsize(*stacka);
	disorder = disordermeter(*stacka);
	if (size <= 10)
	{
		simplesolver(stacka, stackb, counter);
		return ;
	}
	if (size <= 200)
	{
		if (disorder < 2000 || disorder > 8000)
			simplesolver(stacka, stackb, counter);
		else
			mediumsolver(stacka, stackb, counter);
		return ;
	}
	if ((max_value(*stacka) - min_value(*stacka)) > 20000000)
		mediumsolver(stacka, stackb, counter);
	else
		complexsolver(stacka, stackb, counter);
}
