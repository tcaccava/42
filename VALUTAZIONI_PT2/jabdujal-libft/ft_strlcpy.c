/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabdujal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:03:59 by jabdujal          #+#    #+#             */
/*   Updated: 2026/08/17 17:04:18 by jabdujal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	copied;

	i = 0;
	copied = 0;
	while (src[i])
	{
		if (dstsize != 0 && copied < dstsize - 1)
		{
			dst[copied] = src[copied];
			copied++;
		}
		i++;
	}
	if (dstsize != 0)
		dst[copied] = '\0';
	return (i);
}
