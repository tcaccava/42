/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 17:42:11 by keiestre          #+#    #+#             */
/*   Updated: 2026/09/23 17:42:15 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_node;
	t_list	*new_list;
	t_list	*end;

	new_list = NULL;
	end = NULL;
	while (lst != NULL)
	{
		new_node = malloc(sizeof(t_list));
		if (!new_node)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		new_node -> content = f(lst -> content);
		new_node -> next = NULL;
		if (!new_list)
			new_list = new_node;
		else
			end -> next = new_node;
		end = new_node;
		lst = lst -> next;
	}
	return (new_list);
}
