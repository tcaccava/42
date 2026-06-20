/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:45:55 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:45:55 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
	void *memchr(size_t n; const void s[n], int c, size_t n);

DESCRIPTION
		The memchr() function scans the initial n bytes of the memory area
		pointed to by s for the first instance
		of c.  Both c and the bytes of the memory area pointed to by s are
		interpreted as unsigned char.
RETURN VALUE
		The memchr() and memrchr() functions return a pointer to the
		matching byte or NULL if the character  does
		not occur in the given memory area.

		The rawmemchr() function returns a pointer to the matching byte.
*/
#include "libft.h"

void	*ft_memchr(const void *src, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	ch;
	size_t			i;

	i = 0;
	str = (unsigned char *)src;
	ch = (unsigned char)c;
	while (i < n)
	{
		if (str[i] == ch)
			return (str + i);
		i++;
	}
	return (NULL);
}
