/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:44:36 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:44:36 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	SYNOPSIS
		void ft_lstadd_back(t_list **lst, t_list *new);
	PARAMETERS
		lst: The address of a pointer to the first node of a list.
		new: The address of a pointer to the node to be added.
	DESCRIPTION
		Adds the node ’new’ at the end of the list.
*/
#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	last = ft_lstlast(*lst);
	if (!last)
		*lst = new;
	last->next = new;
}
