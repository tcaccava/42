/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:37:39 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/12 17:35:35 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	char	*pointer;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	if (dst_len > size)
		return (size + src_len);
	pointer = dst + dst_len;
	ft_strlcpy(pointer, src, (size - dst_len));
	return (src_len + dst_len);
}

/*#include <stdio.h>

int main()
{
	char	dst[30] = "B";

	printf("%zu", ft_strlcat(dst, "AAAAAAAAA", 1));
	printf("%s", dst);
}*/
