/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:24:21 by ggiument          #+#    #+#             */
/*   Updated: 2026/06/02 16:17:34 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*u;

	i = 0;
	u = s;
	while (i < n)
	{
		if (u[i] == (unsigned char) c)
			return ((void *) & u[i]);
		i++;
	}
	return (NULL);
}
