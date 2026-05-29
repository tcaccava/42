/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:59:43 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/12 11:32:44 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	index;

	index = 0;
	while (src[index] != '\0' && size > 1)
	{
		dst[index] = src[index];
		size--;
		index++;
	}
	if (size > 0)
		dst[index] = '\0';
	return (ft_strlen(src));
}

/*int main()
{
	char	dst[5];
	char	*pointer;

	pointer = dst;
	printf("%d", ft_strlcpy(pointer, "ehy ciao", 5));
	printf("%s", dst);
}*/
