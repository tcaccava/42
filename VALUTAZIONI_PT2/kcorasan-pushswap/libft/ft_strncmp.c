/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slorefic <slorefic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:17:04 by slorefic          #+#    #+#             */
/*   Updated: 2026/05/27 15:43:20 by slorefic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>
#include <string.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		if (s1[i] == '\0')
			return (0);
		i++;
	}
	return (0);
}

/*int main()
{
	printf("%d\n", ft_strncmp("abc", "abc", 3));
	printf("%d\n", strncmp("abc", "abc", 3));

	printf("%d\n", ft_strncmp("abc", "abd", 3));
	printf("%d\n", strncmp("abc", "abd", 3)); //confronta con tab ascii

	printf("%d\n", ft_strncmp("abc", "ab", 3));
	printf("%d\n", strncmp("abc", "ab", 3));

	printf("%d\n", ft_strncmp("ab", "abc", 3));
	printf("%d\n", strncmp("ab", "abc", 3));
}*/
