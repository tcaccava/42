/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 22:21:05 by lvovlas           #+#    #+#             */
/*   Updated: 2026/06/08 12:12:07 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	const unsigned char	*temp_src;
	unsigned char		*temp_dst;

	temp_dst = (unsigned char *) dst;
	temp_src = (const unsigned char *) src;
	if (len == 0 || dst == src)
		return (dst);
	if (temp_dst < temp_src)
	{
		while (len--)
			*temp_dst++ = *temp_src++;
	}
	else
	{
		temp_dst += len;
		temp_src += len;
		while (len--)
			*--temp_dst = *--temp_src;
	}
	return (dst);
}
