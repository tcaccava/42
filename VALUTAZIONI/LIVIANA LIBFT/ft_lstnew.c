/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:23:42 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/16 14:00:08 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *) malloc (sizeof(t_list));
	if (new == NULL)
		return (NULL);
	new -> next = NULL;
	new -> content = content;
	return (new);
}

/*int main()
{
	t_list	*list;
	int		i[] = {1, 2, 3};
	int	*pointer;

	list = ft_lstnew(i);
	pointer = list -> content;
	printf("%d", pointer[2]);
}*/
