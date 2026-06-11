/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscorzon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:49:16 by gscorzon          #+#    #+#             */
/*   Updated: 2026/06/04 19:03:22 by gscorzon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*head;
	t_list	*new;

	head = ft_lstnew(f(lst->content));
	if (!head)
	{
		del(head->content);
		return (lst);
	}
	new = head;
	lst = lst->next;
	while (lst)
	{
		new = ft_lstnew(f(lst->content));
		if (!new)
		{
			del(new->content);
			return (lst);
		}
		ft_lstadd_back(&head, new);
		lst = lst->next;
	}
	return (head);
}
