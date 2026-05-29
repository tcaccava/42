/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 21:18:10 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/10 16:35:01 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del) (void *))
{
	t_list	*pointer;

	if (!lst)
		return ;
	while (*lst != NULL)
	{
		pointer = (*lst)-> next;
		ft_lstdelone(*lst, del);
		*lst = pointer;
	}
	lst = NULL;
}
