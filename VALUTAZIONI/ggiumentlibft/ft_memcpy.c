/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:20:30 by ggiument          #+#    #+#             */
/*   Updated: 2026/05/25 14:55:20 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*p;
	unsigned char	*c;
	size_t			i;

	p = (unsigned char *) src;
	c = (unsigned char *) dst;
	i = 0;
	while (i != n)
	{
		c[i] = p[i];
		i++;
	}
	return (dst);
}
