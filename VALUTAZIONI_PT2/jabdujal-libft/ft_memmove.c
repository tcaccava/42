/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabdujal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:03:08 by jabdujal          #+#    #+#             */
/*   Updated: 2026/08/19 11:34:38 by jabdujal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	copy_reverse(unsigned char *dst, const unsigned char *src,
		size_t len)
{
	while (len > 0)
	{
		len--;
		dst[len] = src[len];
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t				i;
	unsigned char		*dstptr;
	const unsigned char	*srcptr;

	if (!dst && !src)
		return (NULL);
	dstptr = (unsigned char *)dst;
	srcptr = (const unsigned char *)src;
	if (dst < src)
	{
		i = 0;
		while (i < len)
		{
			dstptr[i] = srcptr[i];
			i++;
		}
	}
	else
		copy_reverse(dstptr, srcptr, len);
	return (dst);
}
