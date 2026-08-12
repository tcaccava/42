/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quick.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoda <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 13:43:42 by dmoda             #+#    #+#             */
/*   Updated: 2026/08/11 14:25:57 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

int	pivot(t_dlist **head, int len)
{
	t_dlist	*node;
	int		n;
	int		i;

	i = 0;
	n = 0;
	node = (*head);
	while (i < len)
	{
		n += node->cont;
		i++;
		node = node->next;
	}
	return (n / len);
}

int	ft_rev_divide(t_dlist **head_a, t_dlist **head_b, int len, int n)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (len--)
	{
		if ((*head_b)->cont > n && ++i)
			ft_push(head_a, head_b, 'a');
		else if (++j)
			ft_rotate(head_a, head_b, 1, 'b');
	}
	while (j--)
		ft_rotate(head_a, head_b, 0, 'b');
	return (i);
}

int	ft_divide(t_dlist **head_a, t_dlist **head_b, int len, int n)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (len--)
	{
		if ((*head_a)->cont <= n && ++i)
			ft_push(head_b, head_a, 'b');
		else if (++j)
			ft_rotate(head_a, head_b, 1, 'a');
	}
	while (j--)
		ft_rotate(head_a, head_b, 0, 'a');
	return (i);
}

void	ft_quick(t_dlist **head_a, t_dlist **head_b, int len)
{
	int	n;
	int	len_divide;

	if (len == 1)
		return ;
	if (len == 2)
	{
		if ((*head_a)->cont > (*head_a)->next->cont)
			ft_swap(head_a, NULL, 'a');
		return ;
	}
	n = pivot(head_a, len);
	len_divide = ft_divide(head_a, head_b, len, n);
	ft_quick(head_a, head_b, len - len_divide);
	ft_rev_quick(head_a, head_b, len_divide);
}

void	ft_rev_quick(t_dlist **head_a, t_dlist **head_b, int len)
{
	int	p;
	int	len_divide;

	if (len == 1)
	{
		ft_push(head_a, head_b, 'a');
		return ;
	}
	if (len == 2)
	{
		if ((*head_b)->cont < (*head_b)->next->cont)
			ft_swap(head_b, NULL, 'b');
		ft_push(head_a, head_b, 'a');
		ft_push(head_a, head_b, 'a');
		return ;
	}
	p = pivot(head_b, len);
	len_divide = ft_rev_divide(head_a, head_b, len, p);
	ft_quick(head_a, head_b, len_divide);
	ft_rev_quick(head_a, head_b, len - len_divide);
}
