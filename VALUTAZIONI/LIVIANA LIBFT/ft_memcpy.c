/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:31:24 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/09 15:30:51 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	void	*pointer;

	pointer = dest;
	while (n > 0)
	{
		*(char *)dest = *(const char *)src;
		dest++;
		src++;
		n--;
	}
	return (pointer);
}

/*#include <stdio.h>

int	main()
{
	char	dest[50] = "a shiny white sphere";
	const char	src[] = "Hello";

	ft_memcpy(dest + 3, dest + 2, 2);
	printf("%s", dest);
}*/
