/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disorder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sara <sara@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 02:23:33 by sara              #+#    #+#             */
/*   Updated: 2026/07/24 20:45:58 by sara             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

float	disorder(t_stack *a)
{
	int		mistakes;
	int		total_coppie;
	t_node	*current_i;
	t_node	*current_j;

	mistakes = 0;
	total_coppie = 0;
	current_i = a->top;
	while (current_i != NULL)
	{
		current_j = current_i->next;
		while (current_j != NULL)
		{
			total_coppie++;
			if (current_i->value > current_j->value)
				mistakes++;
			current_j = current_j->next;
		}
		current_i = current_i->next;
	}
	return ((float)mistakes / total_coppie);
}
