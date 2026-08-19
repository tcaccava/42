/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabdujal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 17:04:57 by jabdujal          #+#    #+#             */
/*   Updated: 2026/08/18 17:20:18 by jabdujal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *list)
{
	int		count;
	t_list	*ptr;

	count = 0;
	ptr = list;
	while (ptr)
	{
		count++;
		ptr = ptr->next;
	}
	return (count);
}
