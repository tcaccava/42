/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:36:12 by rodolhop          #+#    #+#             */
/*   Updated: 2025/01/13 13:37:44 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == (unsigned char)c)
			return ((void *)(ptr + i));
		i++;
	}
	return (NULL);
}

/*
int main(void)
{
	char arr[] = "42 School";
	char *res;

	res = ft_memchr(arr, 'S', 5);
	if (res)
		printf("Symbol found: %c\n", *res);
	else
		printf("Symbol not found.\n");
	return (0);
}
*/
