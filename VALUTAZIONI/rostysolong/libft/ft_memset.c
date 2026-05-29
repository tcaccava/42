/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:03:06 by rodolhop          #+#    #+#             */
/*   Updated: 2025/01/13 14:05:50 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*str;

	str = (unsigned char *)s;
	while (n--)
		*str++ = (unsigned char)c;
	return (s);
}

/*
#include <stdio.h>

int	main(void)
{
	char a[] = "privet";
	printf("My ft_memset: %s\n", (char *)ft_memset(a, 'x', 2));

	char a1[] = "poka";
	printf("Original memset: %s\n", (char *)memset(a1, 'x', 2));
	return (0);
}
*/
