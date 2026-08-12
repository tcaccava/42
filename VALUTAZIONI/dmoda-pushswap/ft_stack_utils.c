/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 12:08:01 by grocca            #+#    #+#             */
/*   Updated: 2026/08/11 14:26:41 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_push_swap.h"

t_dlist	*ft_new_node(int cont)
{
	t_dlist	*node;

	node = malloc(sizeof(t_dlist));
	if (!node)
		return (NULL);
	node->next = node;
	node->prev = node;
	node->cont = cont;
	return (node);
}

int	ft_add_node(t_dlist **head_dst, t_dlist *node)
{
	if (!node)
		return (0);
	if (!*head_dst)
	{
		node->next = node;
		node->prev = node;
		*head_dst = node;
		return (1);
	}
	node->next = *head_dst;
	node->prev = (*head_dst)->prev;
	(*head_dst)->prev->next = node;
	(*head_dst)->prev = node;
	return (1);
}

void	ft_free_list(t_dlist **head)
{
	if (head)
	{
		while (*head && (*head) != (*head)->next)
		{
			(*head)->prev = (*head)->prev->prev;
			free((*head)->prev->next);
			(*head)->prev->next = (*head);
		}
		if (*head)
			free(*head);
		*head = NULL;
	}
}

int	ft_stack_len(t_dlist *head)
{
	int		i;
	t_dlist	*current;

	if (!head)
		return (0);
	i = 1;
	current = head;
	while (current->next != head)
	{
		current = current->next;
		i++;
	}
	return (i);
}

t_dlist	**ft_new_head(void)
{
	t_dlist	**head;

	head = malloc(sizeof(t_dlist *));
	if (!head)
	{
		write(2, "Error\n", 6);
		return (NULL);
	}
	*head = NULL;
	return (head);
}
