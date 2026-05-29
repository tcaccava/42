/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:01:11 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/11/13 14:01:51 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	void	*pointer;

	while (n > 0)
	{
		*(char *) dest = *(char *) src;
		if (*(char *)src == c)
		{
			pointer = (char *)src + 1;
			return (pointer);
		}
		dest++;
		src++;
		n--;
	}
	return (NULL);
}

/*int main()
{
	char	dest[20];
	char	src[] = "ehi ciao";

	ft_memccpy(dest, src, 'c', sizeof(src));
	printf("%s", dest);
}*/
