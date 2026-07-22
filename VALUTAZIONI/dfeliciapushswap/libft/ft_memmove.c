/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:49:03 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/25 15:21:43 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*a;
	char	*b;

	if (!dest && !src)
		return (NULL);
	a = (char *) src;
	b = (char *) dest;
	if (b > a)
	{
		while (n)
		{
			n--;
			b[n] = a[n];
		}
	}
	else
		ft_memcpy(dest, src, n);
	return (dest);
}
