/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smattei <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:51:23 by smattei           #+#    #+#             */
/*   Updated: 2025/01/21 15:51:25 by smattei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t count)
{
	size_t			cnt;
	size_t			i;
	unsigned char	*ds;
	unsigned char	*sr;

	cnt = 0;
	i = 1;
	if (!dest || !src)
		return (NULL);
	ds = (unsigned char *)dest;
	sr = (unsigned char *)src;
	if (dest > src)
	{
		i = -1;
		ds += count - 1;
		sr += count -1;
	}
	while (cnt < count)
	{
		*ds = *sr;
		ds += i;
		sr += i;
		cnt ++;
	}
	return (dest);
}
