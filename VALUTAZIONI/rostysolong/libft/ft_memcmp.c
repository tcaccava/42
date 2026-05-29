/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:38:34 by rodolhop          #+#    #+#             */
/*   Updated: 2025/01/13 13:42:14 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

/*
#include <stdio.h>

int	main(void)
{
	unsigned char s1[] = {1, 2, 3, 4, 5};
	unsigned char s2[] = {1, 2, 2, 4, 5};
	int result;

	result = ft_memcmp(s1, s2, 5);
	if (result < 0)
		printf("s1 < s2\n");
	else if (result > 0)
		printf("s1 > s2\n");
	else
		printf("s1 == s2\n");
	return (0);
}
*/
