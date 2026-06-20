/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:03 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:46:03 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	void ft_lstiter(t_list *lst, void (*f)(void *));
PARAMETERS
	lst: The address of a pointer to a node.
	f: The address of the function to apply to each node’s content.
DESCRIPTION
	Iterates through the list ’lst’ and applies the
	function ’f’ to the content of each node.
RETURN VALUES
	None
*/

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*c_lst;

	c_lst = lst;
	while (c_lst)
	{
		f(c_lst->content);
		c_lst = c_lst->next;
	}
}
