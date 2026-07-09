/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caguiari <caguiari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 17:14:06 by caguiari          #+#    #+#             */
/*   Updated: 2026/07/06 17:42:51 by caguiari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_list	*ft_lstnew(int nb)
{
	t_list	*new_node;

	new_node = (t_list *) malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->nb = nb;
	new_node->idx = ND;
	new_node->next = NULL;
	return (new_node);
}

int	ft_lstsize(t_list *first_node)
{
	int	i;

	if (!first_node)
		return (0);
	i = 0;
	while (first_node != NULL)
	{
		i++;
		first_node = first_node->next;
	}
	return (i);
}

void	ft_lstpush(t_list **first_node, t_list *new)
{
	t_list	*ptr;

	if (!new || !first_node)
		return ;
	if (!first_node[0])
	{
		first_node[0] = new;
		return ;
	}
	ptr = first_node[0];
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = new;
}

void	ft_lstprint(t_list *lst_a, t_list *lst_b)
{
	while (lst_a && lst_b)
	{
		ft_putnbr(lst_a->nb);
		write(1, " ", 1);
		ft_putnbr(lst_b->nb);
		write(1, "\n", 1);
		lst_a = lst_a->next;
		lst_b = lst_b->next;
	}
	while (lst_a)
	{
		ft_putnbr(lst_a->nb);
		write(1, "\n", 1);
		lst_a = lst_a->next;
	}
	while (lst_b)
	{
		write(1, "  ", 2);
		ft_putnbr(lst_b->nb);
		write(1, "\n", 1);
		lst_b = lst_b->next;
	}
	write(1, "_ _\na b\n", 8);
}

void	free_lst(t_list *list)
{
	t_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp);
	}
}
