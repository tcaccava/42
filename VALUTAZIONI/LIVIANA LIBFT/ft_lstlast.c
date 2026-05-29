/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 23:32:44 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/10 12:51:41 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst -> next != NULL)
	{
		lst = lst -> next;
	}
	return (lst);
}

/*void	upper(char *str)
{
	printf("%s", str);
}

int main()
{
	t_list	**lst;
	t_list	*l;
	
	lst = malloc (sizeof(t_list *));
	ft_lstadd_front(lst, ft_lstnew("ciao"));
	ft_lstadd_front(lst, ft_lstnew("ehi"));
	l = ft_lstlast(*lst);
	printf("%s", (char *)l -> content);
}*/
