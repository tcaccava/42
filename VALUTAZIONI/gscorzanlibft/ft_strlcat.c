/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscorzon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 18:17:25 by gscorzon          #+#    #+#             */
/*   Updated: 2026/06/09 10:49:20 by gscorzon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dlen;
	size_t	slen;
	size_t	rv;

	dlen = ft_strlen(dst);
	slen = ft_strlen(src);
	if (size > dlen)
		rv = dlen + slen;
	else
		rv = size + slen;
	while (*dst && size > 0)
	{
		dst++;
		size--;
	}
	while (size > 1 && *src)
	{
		size--;
		*dst++ = *src++;
	}
	if (size != 0)
		*dst = 0;
	return (rv);
}
