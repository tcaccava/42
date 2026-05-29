/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:32:56 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/16 15:57:04 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new;
	t_list	*map;

	map = NULL;
	if (!lst || !f || !del)
		return (NULL);
	while (lst != NULL)
	{
		new = ft_lstnew(f(lst -> content));
		if (!new)
		{
			ft_lstclear(&map, del);
			return (NULL);
		}
		ft_lstadd_back(&map, new);
		lst = lst -> next;
	}
	return (map);
}

/*void	*print(void *str)
{
	write(1, "ok", 2);
	return (str);
}

void del(void *str)
{
	return ;
}

int main()
{
	t_list	**lst;

	*lst = ft_lstnew("ciao");
	ft_lstadd_back(lst, ft_lstnew(" come"));
	ft_lstadd_back(lst, ft_lstnew(" stai?"));
	printf("%s", ft_lstmap(*lst, print, del));
}*/
