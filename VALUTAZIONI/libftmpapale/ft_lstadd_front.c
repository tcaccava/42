/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:44:47 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:44:47 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	void ft_lstadd_front(t_list **lst, t_list *new);
PARAMETERS
	lst: The address of a pointer to the first node ofa list.
	new: The address of a pointer to the node to be added
DESCRIPTION
	Adds the node ’new’ at the beginning of the list.
RETURN VALUES
	None
*/
#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = *lst;
	*lst = new;
}
