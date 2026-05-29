/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtozzi-b <mtozzi-b@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:17:28 by mtozzi-b          #+#    #+#             */
/*   Updated: 2025/01/15 17:18:30 by mtozzi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*bytes_s1;
	const unsigned char	*bytes_s2;

	i = 0;
	bytes_s1 = (const unsigned char *)s1;
	bytes_s2 = (const unsigned char *)s2;
	while (i < n)
	{
		if (bytes_s1[i] != bytes_s2[i])
		{
			return (bytes_s1[i] - bytes_s2[i]);
		}
		i++;
	}
	return (0);
}
