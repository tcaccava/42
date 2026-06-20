/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:16 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:47:30 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*SYNOPSIS
		#include <string.h>

		void *memcpy(void *dest, const void *src, size_t n);

DESCRIPTION
		The memcpy() function copies n bytes from memory area
		src to memory area dest.
		The memory areas must not overlap.
		Use memmove(3) if the memory areas do overlap.

RETURN VALUE
		The memcpy() function returns a pointer to dest.
*/
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*pd;
	const unsigned char	*ps;
	size_t				i;

	pd = (unsigned char *)dest;
	ps = (const unsigned char *)src;
	i = 0;
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		pd[i] = ps[i];
		i++;
	}
	return (dest);
}
