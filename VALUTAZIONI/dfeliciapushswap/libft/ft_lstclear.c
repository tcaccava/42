/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:16:09 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/25 15:28:55 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*last;
	t_list	*next;

	if (!lst || !*lst)
	{
		return ;
	}
	last = *lst;
	while (last != NULL)
	{
		next = last->next;
		del(last->content);
		free(last);
		last = next;
	}
	*lst = NULL;
}
