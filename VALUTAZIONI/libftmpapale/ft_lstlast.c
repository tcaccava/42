/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:05 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:46:05 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	t_list *ft_lstlast(t_list *lst);
PARAMETERS
	lst: The beginning of the list.
DESCRIPTION
	Returns the last node of the list.
*/
#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*c_lst;

	if (!lst)
		return (NULL);
	c_lst = lst;
	while (c_lst->next)
		c_lst = c_lst->next;
	return (c_lst);
}
