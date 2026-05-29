/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtozzi-b <mtozzi-b@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 21:55:50 by mtozzi-b          #+#    #+#             */
/*   Updated: 2025/01/18 18:53:21 by mtozzi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*srcc;
	unsigned char	*dstc;

	i = 0;
	if (dst == 0 && src == 0)
		return (0);
	srcc = (unsigned char *)src;
	dstc = (unsigned char *)dst;
	while (i++ < n)
		*(dstc++) = *(srcc++);
	return (dst);
}
