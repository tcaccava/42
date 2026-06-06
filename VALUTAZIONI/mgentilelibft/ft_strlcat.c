/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 17:04:44 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/04 20:05:53 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	lensrc;
	size_t	lendst;

	i = 0;
	j = 0;
	lensrc = 0;
	lendst = 0;
	while (dst[lendst])
		lendst++;
	while (src[lensrc] != '\0')
		lensrc++;
	if (size > lendst)
	{
		i = lendst;
		while (i < size - 1 && src[j] != '\0')
			dst[i++] = src[j++];
		dst[i] = '\0';
		return (lendst + lensrc);
	}
	else
		return (size + lensrc);
}
/*
int	main(void)
{
	char	src[] = "ciao miao";
	char	dst[] = "il gatto fa ";

	ft_strlcat(dst, src, 17);
	printf("%s\n", dst);
}
*/
