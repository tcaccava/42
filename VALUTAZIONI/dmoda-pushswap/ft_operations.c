/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operations.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 14:37:32 by grocca            #+#    #+#             */
/*   Updated: 2026/08/11 14:07:57 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

void	ft_rotate(t_dlist **head_a, t_dlist **head_b, int rotation, char c)
{
	ft_bench_r(rotation, c);
	if (rotation)
	{
		if (c == 'a')
			write(1, "ra\n", 3);
		else if (c == 'b')
			write(1, "rb\n", 3);
		else if (c == 'r')
			write(1, "rr\n", 3);
		if (c == 'a' || c == 'r')
			*head_a = (*head_a)->next;
		if (c == 'b' || c == 'r')
			*head_b = (*head_b)->next;
		return ;
	}
	if (c == 'a')
		write(1, "rra\n", 4);
	else if (c == 'b')
		write(1, "rrb\n", 4);
	else if (c == 'r')
		write(1, "rrr\n", 4);
	if (c == 'a' || c == 'r')
		*head_a = (*head_a)->prev;
	if (c == 'b' || c == 'r')
		*head_b = (*head_b)->prev;
}

void	ft_push(t_dlist **head_dst, t_dlist **head_src, char c)
{
	t_dlist	*next_head;

	if (!*head_src)
		return ;
	if (c == 'a')
		write(1, "pa\n", 3);
	else
		write(1, "pb\n", 3);
	if (*head_src != (*head_src)->next)
	{
		(*head_src)->prev->next = (*head_src)->next;
		(*head_src)->next->prev = (*head_src)->prev;
		next_head = (*head_src)->next;
		ft_add_node(head_dst, *head_src);
		*head_dst = (*head_dst)->prev;
		*head_src = next_head;
	}
	else
	{
		ft_add_node(head_dst, *head_src);
		*head_dst = (*head_dst)->prev;
		*head_src = NULL;
	}
	ft_bench_sp(0, c);
}

void	ft_swap(t_dlist **head, t_dlist **head_snd, char c)
{
	if (!head || !(*head) || *head == (*head)->next)
		return ;
	if (c == 'a')
		write(1, "sa\n", 3);
	else if (c == 'b')
		write(1, "sb\n", 3);
	else if (c == 's')
		write(1, "ss\n", 3);
	if (c == 's')
		ft_swap(head_snd, NULL, 'x');
	if (*head == (*head)->next->next)
		*head = (*head)->next;
	else
	{
		(*head)->next = (*head)->next->next;
		(*head)->prev->next = (*head)->next->prev;
		(*head)->next->prev = *head;
		(*head)->prev->next->next = *head;
		(*head)->prev->next->prev = (*head)->prev;
		(*head)->prev = (*head)->prev->next;
		*head = (*head)->prev;
	}
	ft_bench_sp(1, c);
}
