/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 16:30:22 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/06 17:59:42 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_ordered(t_list *lst)
{
	if (!lst->next)
		return (TRUE);
	while (lst->next)
	{
		if (lst->nb > lst->next->nb)
			return (FALSE);
		lst = lst->next;
	}
	return (TRUE);
}

int	is_reverse_ordered(t_list *lst)
{
	if (!lst->next)
		return (TRUE);
	while (lst->next)
	{
		if (lst->nb < lst->next->nb)
			return (FALSE);
		lst = lst->next;
	}
	return (TRUE);
}

void	rotate_to_order(t_list **a, t_count *count)
{
	int	distance;
	int	len;

	distance = target_distance(lesser_number(*a),*a);
	len = ft_lstsize(*a);
	if (distance > (len / 2))
	{
		while (!is_ordered(*a))
			reverse_rotate(a, 'a', count);
	}
	else
		while (!is_ordered(*a))
			rotate(a, 'a', count);
}
