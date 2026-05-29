/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrunial <ibrunial@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 10:46:51 by ibrunial          #+#    #+#             */
/*   Updated: 2024/12/11 15:22:47 by ibrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *src, int c, size_t n)
{
	unsigned char	*c_src;
	unsigned char	uc_c;
	size_t			i;

	if (n == 0)
		return (NULL);
	c_src = (unsigned char *)src;
	uc_c = (unsigned char)c;
	i = 0;
	while (i < n && *c_src != uc_c)
	{
		c_src++;
		i++;
	}
	if (*c_src == uc_c && i < n)
		return ((void *)c_src);
	return (NULL);
}
