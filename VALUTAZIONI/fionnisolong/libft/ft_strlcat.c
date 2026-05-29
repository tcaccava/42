/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:07:07 by fionni            #+#    #+#             */
/*   Updated: 2024/12/31 15:53:08 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	d;
	size_t	s;
	size_t	i;
	size_t	j;

	d = ft_strlen(dest);
	s = ft_strlen(src);
	i = 0;
	j = d;
	if (d < size - 1 && size > 0)
	{
		while (src[i] != '\0' && d + i < size - 1)
		{
			dest[j] = src[i];
			j++;
			i++;
		}
		dest[j] = 0;
	}
	if (d >= size)
		d = size;
	return (d + s);
}
