/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 17:42:51 by keiestre          #+#    #+#             */
/*   Updated: 2026/09/23 17:42:55 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	unsigned char		*k;
	unsigned char		j;
	size_t				i;

	k = (unsigned char *)str;
	j = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (k[i] == j)
			return ((unsigned char *)&k[i]);
		i++;
	}
	return (NULL);
}
