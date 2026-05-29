/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 13:17:48 by gcollet           #+#    #+#             */
/*   Updated: 2025/06/26 00:38:23 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Supprime et libère la mémoire de l’élément passé en paramètre, et de tous 
les élements qui suivent, à l’aide de del et de free(3) Enfin, le pointeur 
initial doit être mis à NULL. */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*temp;

	if (lst == NULL)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		(*lst) = temp;
	}
}
