/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:14:12 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/06 17:43:26 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	swap(t_list **lst, char c, t_count *count)
{
	t_list	*one;

	if (!lst || !*lst || !(*lst)->next)
		return ;
	if (c == 'a')
	{
		write(1, "sa\n", 3);
		count->sa += 1;
		count->total += 1;
	}
	else if (c == 'b')
	{
		write(1, "sb\n", 3);
		count->sb += 1;
		count->total += 1;
	}
	one = *lst;
	*lst = (*lst)->next;
	one->next = (*lst)->next;
	(*lst)->next = one;
}

void	push(t_list **lst_dest, t_list **lst_src, char c, t_count *count)
{
	t_list	*tmp_a;

	if (!lst_src || !*lst_src)
		return ;
	if (c == 'a')
	{
		write(1, "pa\n", 3);
		count->pa += 1;
	}
	else
	{
		write(1, "pb\n", 3);
		count->pb += 1;
	}
	count->total += 1;
	tmp_a = *lst_src;
	*lst_src = (*lst_src)->next;
	if (!*lst_dest)
	{
		*lst_dest = tmp_a;
		(*lst_dest)->next = NULL;
		return ;
	}
	tmp_a->next = *lst_dest;
	*lst_dest = tmp_a;
}

void	rotate(t_list **lst, char c, t_count *count)
{
	t_list	*first;
	t_list	*tmp;

	if (!lst || !*lst || !(*lst)->next)
		return ;
	if (c == 'a')
	{
		write(1, "ra\n", 3);
		count->ra += 1;
		count->total += 1;
	}
	else if (c == 'b')
	{
		write(1, "rb\n", 3);
		count->rb += 1;
		count->total += 1;
	}
	first = *lst;
	tmp = *lst;
	*lst = ((*lst)->next);
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = first;
	first->next = NULL;
}

void	reverse_rotate(t_list **lst, char c, t_count *count)
{
	t_list	*last;
	t_list	*prev;
	t_list	*first;

	if (!lst || !*lst || !(*lst)->next)
		return ;
	if (c == 'a' && ++count->rra)
		write(1, "rra\n", 4);
	else if (c == 'b' && ++count->rrb)
		write(1, "rrb\n", 4);
	count->total += 1;
	last = *lst;
	first = *lst;
	prev = NULL;
	while (last->next)
	{
		prev = last;
		last = last->next;
	}
	prev->next = NULL;
	last->next = first;
	*lst = last;
}
