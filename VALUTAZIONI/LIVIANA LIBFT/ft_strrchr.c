/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:46:44 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/12 11:00:24 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*pointer;
	char	a;

	pointer = NULL;
	a = c;
	while (*s)
	{
		if (*s == a)
			pointer = (char *)s;
		s++;
	}
	if (*s == a)
		pointer = (char *)s;
	return (pointer);
}

/*int main()
{
	printf("%s", ft_strrchr("ciao, come stcai?", 'c'));
}*/
