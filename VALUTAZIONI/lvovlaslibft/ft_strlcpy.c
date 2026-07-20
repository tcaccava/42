/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 17:25:05 by lvovlas           #+#    #+#             */
/*   Updated: 2026/07/16 13:52:58 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	len;

	len = ft_strlen(src);
	if (dstsize)
	{
		while (dstsize > 1 && *src)
		{
			*dst++ = *src++;
			dstsize--;
		}
		if (dstsize)
			*dst = '\0';
	}
	return (len);
}
