/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smattei <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 15:57:15 by smattei           #+#    #+#             */
/*   Updated: 2025/01/21 15:57:26 by smattei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	i_dst;
	size_t	i_src;

	i = 0;
	if (!dest)
		return (0);
	i_dst = ft_strlen(dest);
	i_src = ft_strlen(src);
	if (size <= i_dst)
		return (size + i_src);
	while (src[i] != '\0' && (i_dst + i) < size - 1)
	{
		dest[i + i_dst] = src[i];
		i ++;
	}
	dest[i + i_dst] = '\0';
	return (i_dst + i_src);
}
