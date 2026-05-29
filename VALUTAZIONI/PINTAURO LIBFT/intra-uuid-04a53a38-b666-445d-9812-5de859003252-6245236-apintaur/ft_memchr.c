/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:07:19 by apintaur          #+#    #+#             */
/*   Updated: 2024/12/16 22:27:22 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str_tmp;
	unsigned char	char_tmp;
	size_t			i;

	str_tmp = (unsigned char *)s;
	char_tmp = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (str_tmp[i] == char_tmp)
			return ((void *)&str_tmp[i]);
		i++;
	}
	return (NULL);
}
