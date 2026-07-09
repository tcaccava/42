/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   greatest_or_smallest.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 16:25:39 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/06 17:58:40 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_smallest_number(int nb, t_list *lst)
{
	while (lst)
	{
		if (lst->nb < nb)
			return (FALSE);
		lst = lst->next;
	}
	return (TRUE);
}

int	is_greatest_number(int nb, t_list *lst)
{
	while (lst)
	{
		if (lst->nb > nb)
			return (FALSE);
		lst = lst->next;
	}
	return (TRUE);
}

int	lesser_number(t_list *lst)
{
	int	min;

	min = lst->nb;
	lst = lst->next;
	while (lst)
	{
		if (lst->nb < min)
			min = lst->nb;
		lst = lst->next;
	}
	return (min);
}

int	greatest_number(t_list *lst)
{
	int	max;

	max = lst->nb;
	lst = lst->next;
	while (lst)
	{
		if (lst->nb > max)
			max = lst->nb;
		lst = lst->next;
	}
	return (max);
}

int	prev_greatest_number(t_list *lst)
{
	int	prev;
	int	greatest;

	prev = lst->nb;
	if (ft_lstsize(lst) == 2)
	{
		if (lst->next->nb < prev)
			return (lst->next->nb);
	}
	greatest = greatest_number(lst);
	lst = lst->next;
	while (lst)
	{
		if (lst->nb != greatest)
		{
			if (lst->nb > prev)
				prev = lst->nb;
		}
		lst = lst->next;
	}
	return (prev);
}
