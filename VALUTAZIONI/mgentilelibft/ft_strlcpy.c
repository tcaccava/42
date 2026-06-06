/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 14:52:57 by mgentile          #+#    #+#             */
/*   Updated: 2026/05/30 20:15:20 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	int		i;
	size_t	x;

	i = 0;
	while (src[i] != '\0')
		i++;
	x = 0;
	while (size > 0 && x < size - 1 && src[x] != '\0')
	{
		dst[x] = src[x];
		x++;
	}
	if (size > 0)
		dst[x] = '\0';
	return (i);
}

/*int	main(void)
{
	char dst[3] = {0};
	char src[] = "ciao bru";

	ft_strlcpy(dst, src, 5);
	printf("%s\n", dst);
}*/
