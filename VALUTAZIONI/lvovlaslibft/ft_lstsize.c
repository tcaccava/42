/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:06:02 by lvovlas           #+#    #+#             */
/*   Updated: 2026/06/09 19:18:10 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*temp;
	int		len;

	if (!lst)
		return (0);
	temp = lst->next;
	len = 1;
	while (temp)
	{
		temp = temp->next;
		len++;
	}
	return (len);
}
