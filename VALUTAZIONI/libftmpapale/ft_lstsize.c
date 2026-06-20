/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:13 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:46:13 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	int ft_lstsize(t_list *lst);
PARAMETERS
	lst: The beginning of the list
DESCRIPTION
	Counts the number of nodes in the list.
*/
#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*c_lst;
	int		i;

	c_lst = lst;
	i = 0;
	while (c_lst)
	{
		c_lst = c_lst->next;
		i++;
	}
	return (i);
}
