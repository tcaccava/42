/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 15:49:55 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/25 17:25:41 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = ft_strlen(dst);
	if (size > n)
	{
		while ((n + i) < size - 1 && src[i] != '\0')
		{
			dst[n + i] = src[i];
			i++;
		}
		dst[n + i] = '\0';
	}
	if (size <= n)
		return (size + ft_strlen(src));
	return (n + ft_strlen(src));
}
