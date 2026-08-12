/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bubble.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoda <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 13:56:49 by dmoda             #+#    #+#             */
/*   Updated: 2026/08/11 13:56:51 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

void	ft_bubble(t_dlist **head_a, t_dlist **head_b)
{
	if (!head_a || !*head_a || ft_disorder(*head_a, ft_stack_len(*head_a)) == 0)
		return ;
	while (ft_disorder(*head_a, ft_stack_len(*head_a)))
	{
		while (*head_a && *head_a != (*head_a)->next)
		{
			if ((*head_a)->cont <= (*head_a)->next->cont)
				ft_push(head_b, head_a, 'b');
			else
			{
				ft_swap(head_a, NULL, 'a');
				ft_push(head_b, head_a, 'b');
			}
		}
		while (*head_b)
		{
			if ((*head_b)->cont >= (*head_b)->next->cont)
				ft_push(head_a, head_b, 'a');
			else
			{
				ft_swap(head_b, NULL, 'b');
				ft_push(head_a, head_b, 'a');
			}
		}
	}
}
