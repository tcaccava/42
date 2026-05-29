/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:42:59 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/12 17:39:25 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (*(unsigned char *)(s1 + i) != *(unsigned char *)(s2 + i))
		{
			return (*(unsigned char *)(s1 + i) - *(unsigned char *)(s2 + i));
		}
		i++;
	}
	return (0);
}

/*#include <stdio.h>
#include <string.h>

int main(int av, char **ag)
{
	char s1[] = {1, 2, 42, 1};
	char s2[] = {1, 2, 42, 1};

	printf("%d", ft_memcmp(s1, s2, 4));
	printf("%d", memcmp(s1, s2, 4));
}*/
