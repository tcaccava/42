/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 23:03:36 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/10 12:17:34 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst == NULL || new == NULL)
		return ;
	new -> next = *lst;
	*lst = new;
}

/*int main()
{
	t_list	**lst;
	t_list	*p1;
	t_list	*p2;

	p1 -> content = "ciao";
	p1 -> next = p2;
	lst[0] = p1;
	p2 -> content = "ehi";
	p2 -> next = NULL;
	ft_lstadd_front(lst, p2);
	printf("%s", lst[0] -> content);
	printf("%s", lst[0] -> next -> content);
}*/
