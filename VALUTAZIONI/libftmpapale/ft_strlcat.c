/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:46:57 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:46:57 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// size è la grandezza del buffer
// dest_len è quanti caratteri ci sono già dentro dest
size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	i;
	size_t	j;

	src_len = ft_strlen((char *)src);
	dest_len = ft_strlen((char *)dest);
	i = 0;
	j = dest_len;
	if (size <= dest_len)
		return (size + src_len);
	else
	{
		while (src[i] && j < size - 1)
		{
			dest[j] = src[i];
			i++;
			j++;
		}
	}
	dest[j] = '\0';
	return (src_len + dest_len);
}
