/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:55:39 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/21 17:58:11 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sa(t_stack *a)
{
	int	temp;

	if (a && a->next)
	{
		temp = a->value;
		a->value = a->next->value;
		a->next->value = temp;
		write(1, "sa\n", 3);
	}
}

void	silent_sa(t_stack *a)
{
	int	temp;

	if (a && a->next)
	{
		temp = a->value;
		a->value = a->next->value;
		a->next->value = temp;
	}
}

void	sb(t_stack *b)
{
	int	temp;

	if (b && b->next)
	{
		temp = b->value;
		b->value = b->next->value;
		b->next->value = temp;
		write(1, "sb\n", 3);
	}
}

void	silent_sb(t_stack *b)
{
	int	temp;

	if (b && b->next)
	{
		temp = b->value;
		b->value = b->next->value;
		b->next->value = temp;
	}
}

void	ss(t_stack *a, t_stack *b)
{
	int	temp;

	if (a && a->next)
	{
		temp = a->value;
		a->value = a->next->value;
		a->next->value = temp;
	}
	if (b && b->next)
	{
		temp = b->value;
		b->value = b->next->value;
		b->next->value = temp;
	}
	write(1, "ss\n", 3);
}
