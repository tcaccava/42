/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 18:00:18 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/25 17:28:09 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	char	*mem1;
	char	*mem2;
	size_t	i;

	i = 0;
	mem1 = (char *)s1;
	mem2 = (char *)s2;
	while (i < n)
	{
		if (mem1[i] != mem2[i])
			return ((unsigned char)mem1[i] - (unsigned char)mem2[i]);
		i++;
	}
	return (0);
}
