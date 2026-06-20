/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 18:02:40 by mpapale           #+#    #+#             */
/*   Updated: 2026/05/19 18:02:43 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*SYNOPSIS
		#include <string.h>

		void *memset(void *s, int c, size_t n);
	(puntatore a dove va fatto il cambiamento, valore da inserire,
	lunghezza del cambiamento)
DESCRIPTION
		The  memset() function fills the first n bytes of the memory
		area pointed to by s with the constant byte c.

RETURN VALUE
		The memset() function returns a pointer to the memory area s.*/
#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)s;
	while (i < n)
		ptr[i++] = (unsigned char)c;
	return (s);
}
