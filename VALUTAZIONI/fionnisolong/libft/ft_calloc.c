/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 16:11:29 by fionni            #+#    #+#             */
/*   Updated: 2024/12/31 15:30:51 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	s;
	void	*dst;

	if (size != 0 && nmemb > SIZE_MAX / size)
	{
		return (NULL);
	}
	s = (nmemb * size);
	dst = malloc(s);
	if (dst == NULL)
		return (NULL);
	ft_bzero(dst, s);
	return (dst);
}
