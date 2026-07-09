/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_index.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:13:54 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/06 18:01:33 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	greatest_index(t_list *lst)
{
	int	max;

	max = lst->idx;
	lst = lst->next;
	while (lst)
	{
		if (lst->idx > max)
			max = lst->idx;
		lst = lst->next;
	}
	return (max);
}

int	target_distance(int nb, t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		if (lst->nb == nb)
			return (i);
		lst = lst->next;
		i++;
	}
	return (i);
}

int	target_idx_distance(int target, t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		if (lst->idx == target)
			return (i);
		lst = lst->next;
		i++;
	}
	return (-1);
}

static int	lesser_nb_needing_index(t_list *lst)
{
	int	min;

	min = INT_MAX;
	while (lst)
	{
		if (lst->idx == ND && lst->nb < min)
			min = lst->nb;
		lst = lst->next;
	}
	return (min);
}

void	index_list(t_list *lst)
{
	t_list	*start;
	int		i;
	int		min;
	int		size;

	i = 0;
	size = ft_lstsize(lst);
	start = lst;
	while (i < size)
	{
		lst = start;
		while (lst)
		{
			min = lesser_nb_needing_index(start);
			if (lst->nb == min)
			{
				lst->idx = i;
				i++;
			}
			lst = lst->next;
		}
	}
}
