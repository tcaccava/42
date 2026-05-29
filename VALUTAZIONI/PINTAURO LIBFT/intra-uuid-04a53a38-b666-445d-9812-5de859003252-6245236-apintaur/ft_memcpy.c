/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:56:54 by apintaur          #+#    #+#             */
/*   Updated: 2024/12/15 20:18:01 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t			i;

	i = 0;
	if (dest != NULL && src != NULL)
	{
		d = (unsigned char *)dest;
		s = (unsigned char *)src;
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	return (dest);
}
