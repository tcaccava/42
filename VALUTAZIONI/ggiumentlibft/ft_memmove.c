/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:59:49 by ggiument          #+#    #+#             */
/*   Updated: 2026/05/21 19:54:20 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*p;
	unsigned char	*c;
	size_t			i;

	if (!src && !dst)
		return (dst);
	p = (unsigned char *) src;
	c = (unsigned char *) dst;
	i = 0;
	if (c < p)
	{
		while (i != len)
		{
			c[i] = p[i];
			i++;
		}
	}
	else
	{
		while (len--)
			c[len] = p[len];
	}
	return (dst);
}
