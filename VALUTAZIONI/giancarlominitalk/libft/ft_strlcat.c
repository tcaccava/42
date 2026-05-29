/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 15:07:00 by girizzi           #+#    #+#             */
/*   Updated: 2025/01/02 20:32:50 by girizzi          ###   ########.fr       */
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
