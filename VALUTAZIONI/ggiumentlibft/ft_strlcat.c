/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 18:23:30 by ggiument          #+#    #+#             */
/*   Updated: 2026/05/25 15:03:25 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	u;
	size_t	s;

	i = 0;
	u = 0;
	s = 0;
	while (src[i])
		i++;
	if (dstsize == 0)
		return (i);
	while (dst[u] && u < dstsize)
		u++;
	if (u < dstsize)
	{
		while (src[s] && (u + s < dstsize - 1))
		{
			dst[u + s] = src[s];
			s++;
		}
		dst[u + s] = '\0';
	}
	return (u + i);
}
