/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:18:40 by lvovlas           #+#    #+#             */
/*   Updated: 2026/06/08 12:10:31 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*temp_s;
	unsigned char		temp_c;

	temp_c = (unsigned char) c;
	temp_s = (const unsigned char *) s;
	while (n--)
	{
		if (*temp_s++ == temp_c)
			return ((void *)--temp_s);
	}
	return (NULL);
}
