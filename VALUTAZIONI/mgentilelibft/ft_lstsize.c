/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 11:32:14 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/05 17:03:33 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

// int main()
// {
// 	t_list *node;
// 	t_list *new;
// 	printf("Ho aggiunto il nodo 0\n");
// 	node = ft_lstnew(NULL);

// 	int i = 1;
// 	while (i < 7)
// 	{
// 		new = ft_lstnew(NULL);
// 		ft_lstadd_front(&node, new);
// 		printf("Sto aggiungendo il nodo %d\n", i);
// 		i++;
// 	}

// 	int c = ft_lstsize(node);
// 	printf("La lista ha %d\n", c);

// }
