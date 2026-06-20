/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:15:24 by mpapale           #+#    #+#             */
/*   Updated: 2026/05/19 18:21:16 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*SYNOPSIS
		#include <strings.h>

		void bzero(void *s, size_t n);
		puntatore a dove devo cancellare, qaunto devo cancellare,
		#include <string.h>

DESCRIPTION
		The bzero() function erases the data in the n bytes of the
		memory starting at the location pointed to by s, by writing
		zeros (bytes containing '\0') to that area.
		.0

*/
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)s;
	while (i < n)
		ptr[i++] = 0;
}

/*int	main(void)
{
		char    str1[16] = "abcdefghilmno";

		//ft_bzero
		printf("%s\n", str1);
		ft_bzero(str1+6, 10);
		printf("%s\n", str1);
}*/
