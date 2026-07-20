/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 20:24:24 by lvovlas           #+#    #+#             */
/*   Updated: 2026/06/08 12:13:49 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const unsigned char	*temp_src;
	unsigned char		*temp_dst;

	temp_dst = dst;
	temp_src = src;
	if (dst == NULL && src == NULL)
		return (NULL);
	while (n--)
		*temp_dst++ = *temp_src++;
	return (dst);
}
