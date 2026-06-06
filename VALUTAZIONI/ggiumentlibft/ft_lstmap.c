/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 20:14:40 by ggiument          #+#    #+#             */
/*   Updated: 2026/06/04 19:05:43 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_nd;
	t_list	*new_lst;

	new_lst = NULL;
	while (lst)
	{
		new_nd = ft_lstnew(f(lst->content));
		if (!new_nd)
		{
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new_nd);
		lst = lst->next;
	}
	return (new_lst);
}
