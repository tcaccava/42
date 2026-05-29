/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrunial <ibrunial@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 12:26:23 by ibrunial          #+#    #+#             */
/*   Updated: 2024/12/13 12:36:03 by ibrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*cp_begin;

	if (lst == NULL || new == NULL)
		return ;
	cp_begin = *lst;
	if (cp_begin == NULL)
	{
		*lst = new;
		return ;
	}
	while (cp_begin->next != NULL)
		cp_begin = cp_begin->next;
	cp_begin->next = new;
}
