/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:41 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:46:41 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	void ft_lstdelone(t_list *lst, void (*del)(void*));
PARAMETERS
	lst: The node to free.
	del: The address of the function used to delete
	the content.
DESCRIPTION
	Takes a node as parameter and frees its content
	using the function ’del’. Free the node itself but
	does NOT free the next node.
RETURN VALUES
	None
*/

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	del(lst->content);
	free(lst);
}
