/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:18 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:47:29 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*SYNOPSIS
		#include <string.h>

		void *memmove(void *dest, const void *src, size_t n);

DESCRIPTION
		The  memmove()  function  copies n bytes from memory
		area src to memory area dest.  The memory
		areas may overlap: copying takes place as though the
		bytes in src are first copied into a tem‐
		porary  array that does not overlap src or dest, and
		the bytes are then copied from the tempo‐
		rary array to dest.

RETURN VALUE
		The memmove() function returns a pointer to dest.
*/
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*pd;
	const unsigned char	*ps;
	size_t				i;

	pd = (unsigned char *)dest;
	ps = (const unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	if (pd > ps)
	{
		i = n;
		while (i-- > 0)
			pd[i] = ps[i];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			pd[i] = ps[i];
			i++;
		}
	}
	return (dest);
}
