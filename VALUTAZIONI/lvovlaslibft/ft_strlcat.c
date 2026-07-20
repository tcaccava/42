/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 13:04:04 by lvovlas           #+#    #+#             */
/*   Updated: 2026/06/08 12:18:46 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;

	dst_len = 0;
	src_len = ft_strlen(src);
	i = 0;
	while (dst_len < dstsize && dst[dst_len])
		dst_len++;
	if (dst_len < dstsize)
	{
		while (dstsize - dst_len - i > 1 && *src)
		{
			dst[dst_len + i] = *src++;
			i++;
		}
		dst[dst_len + i] = '\0';
	}
	return (dst_len + src_len);
}
