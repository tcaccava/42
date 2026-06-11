/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdecarli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:34:55 by mdecarli          #+#    #+#             */
/*   Updated: 2026/06/02 16:23:20 by mdecarli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t n)
{
	size_t	len_dest;
	size_t	len_src;
	size_t	i;
	size_t	j;

	len_dest = 0;
	while (dest[len_dest] != '\0' && len_dest < n)
		len_dest++;
	len_src = 0;
	while (src[len_src] != '\0')
		len_src++;
	if (len_dest >= n)
		return (n + len_src);
	i = len_dest;
	j = 0;
	while (src[j] != '\0' && (i + j + 1) < n)
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (len_dest + len_src);
}
/*int	main(void)
{
	char	dst[12] = "miaomiao";
	char	src[12] = "ciaociao";

	ft_strlcat(dst, src, 12);
	printf("%s\n", dst);
	return (0);
}*/
