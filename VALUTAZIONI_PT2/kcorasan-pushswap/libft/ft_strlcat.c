/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slorefic <slorefic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:15:36 by slorefic          #+#    #+#             */
/*   Updated: 2026/05/27 15:36:05 by slorefic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>
#include <string.h>

size_t	ft_strlcat(char *dest, const char *src, size_t destsize)
{
	size_t	i;
	size_t	src_i;
	size_t	dest_i;

	i = 0;
	src_i = 0;
	dest_i = 0;
	while (dest[dest_i] && dest_i < destsize)
	{
		dest_i++;
	}
	while (src[src_i])
	{
		src_i++;
	}
	if (dest_i == destsize)
		return (dest_i + src_i);
	while (src[i] && (dest_i + i) < destsize - 1)
	{
		dest[dest_i + i] = src[i];
		i++;
	}
	dest[dest_i + i] = '\0';
	return (dest_i + src_i);
}

/*int main ()
{
	char src[] = "caparezza";
	char dest[14] = "ciao";
	size_t risu;

	risu = ft_strlcat(dest, src, sizeof(dest));

	printf("si parte da qua: %s\n",src);
	printf("si arriva qua: %s\n", dest);
	printf("quindi sono: %zu\n",risu);
}*/
