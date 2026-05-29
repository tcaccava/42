/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smattei <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:50:58 by smattei           #+#    #+#             */
/*   Updated: 2025/01/21 15:51:00 by smattei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*s_s1;
	unsigned char	*s_s2;

	s_s1 = (unsigned char *)s1;
	s_s2 = (unsigned char *)s2;
	if (!s1 || !s2)
		return (0);
	if (n == 0)
		return (0);
	while (n > 0)
	{
		if (*s_s1 != *s_s2)
			return ((int)*s_s1 - *s_s2);
		s_s1 ++;
		s_s2 ++;
		n --;
	}
	return (0);
}
