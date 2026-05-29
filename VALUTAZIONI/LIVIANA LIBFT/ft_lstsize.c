/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 23:13:47 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/10 12:17:03 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	len;

	len = 0;
	if (lst == NULL)
		return (len);
	while (lst != NULL)
	{
		len++;
		lst = lst -> next;
	}
	return (len);
}

/*int main()
{
	t_list	**lst;

	lst = malloc (sizeof(t_list *));
	ft_lstadd_front(lst, ft_lstnew("ciao"));
	ft_lstadd_front(lst, ft_lstnew("ehi"));
	printf("%d", ft_lstsize(*lst));
}*/
