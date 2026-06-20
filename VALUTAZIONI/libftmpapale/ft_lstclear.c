/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:38 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:46:38 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	void ft_lstclear(t_list **lst, void (*del)(void*));
PARAMETERS
	lst: The address of a pointer to a node.
	del: The address of the function used to delete
	the content of the node.
DESCRIPTION
	Deletes and frees the given node and all its
	successors, using the function ’del’ and free(3).
	Finally, set the pointer to the list to NULL.
RETURN VALUES
	None
*/

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*c_lst;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		c_lst = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = c_lst;
	}
	*lst = NULL;
}
