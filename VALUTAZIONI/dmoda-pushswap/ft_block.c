/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_block.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 12:51:51 by grocca            #+#    #+#             */
/*   Updated: 2026/08/11 15:05:48 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

void	ft_next_mv(t_dlist **head_a, t_dlist **head_b, int i, int *len)
{
	char	c;

	c = 'r';
	if (*head_a && (*head_a)->cont > (*head_a)->next->cont)
		c = 'a';
	if (*head_b && (*head_b)->cont > (*head_b)->next->cont)
	{
		if (c == 'a')
			c = 's';
		else
			ft_swap(head_b, head_a, 'b');
	}
	if (c != 'r')
		ft_swap(head_a, head_b, c);
	c = 'r';
	if ((len[0] - len[1]) > 0 && i <= (len[0] - len[1]))
		c = 'a';
	else if ((len[1] - len[0]) > 0 && i <= (len[1] - len[0]))
		c = 'b';
	ft_rotate(head_a, head_b, 1, c);
}

void	ft_bubbletwo(t_dlist **head_a, t_dlist **head_b, int len_a, int len_b)
{
	int		i;
	char	c;
	int		len[2];

	len[0] = len_a;
	len[1] = len_b;
	while (ft_disorder(*head_b, len_b) || ft_disorder(*head_a, len_a))
	{
		i = ft_max(len_a, len_b) - 1;
		while (i--)
			ft_next_mv(head_a, head_b, i, len);
		while (i++ < (ft_max(len_a, len_b) - 2))
		{
			c = 'r';
			if ((len_a - len_b) > 0 && i <= (len_a - len_b))
				c = 'a';
			else if ((len_b - len_a) > 0 && i <= (len_b - len_a))
				c = 'b';
			ft_rotate(head_a, head_b, 0, c);
		}
	}
}

int	ft_rotcalc(t_dlist **head_a, t_dlist **head_b)
{
	t_dlist	*n1;
	t_dlist	*n2;

	n1 = (*head_a)->next;
	n2 = (*head_a)->prev;
	while (*head_b && n1 != *head_a)
	{
		if ((*head_b)->cont <= n1->cont && (*head_b)->cont >= n1->prev->cont)
			return (0);
		if ((*head_b)->cont <= n2->cont && (*head_b)->cont >= n2->prev->cont)
			return (1);
		n1 = n1->next;
		n2 = n2->prev;
	}
	while (1)
	{
		if (n1->cont <= n1->prev->cont)
			return (0);
		if (n2->cont <= n2->prev->cont)
			return (1);
		n1 = n1->next;
		n2 = n2->prev;
	}
}

void	ft_sort(t_dlist **a, t_dlist **b)
{
	int	i;

	i = 0;
	ft_push(a, b, 'a');
	if (*b)
		ft_push(a, b, 'a');
	while (*b)
	{
		if (ft_condition(a, b))
		{
			ft_push(a, b, 'a');
			i = -1;
		}
		else
		{
			if (i == -1)
				i = ft_rotcalc(a, b);
			ft_rotate(a, b, i - 1, 'a');
		}
	}
	i = ft_rotcalc(a, b);
	while ((*a)->prev->cont < (*a)->cont)
		ft_rotate(a, b, i - 1, 'a');
}

void	ft_block(t_dlist **head_a, t_dlist **head_b, int size)
{
	int			len_a;
	int			i;

	len_a = ft_stack_len(*head_a);
	if (len_a > size)
	{
		i = size;
		while (i--)
			ft_push(head_b, head_a, 'b');
		if ((len_a - size) > size)
			len_a = 2 * size;
		ft_bubbletwo(head_a, head_b, len_a - size, size);
	}
	else
		ft_bubbletwo(head_a, head_b, len_a, 0);
	while (len_a--)
		ft_push(head_b, head_a, 'b');
	if (*head_a)
		ft_block(head_a, head_b, size);
	if (*head_b)
		ft_sort(head_a, head_b);
}
