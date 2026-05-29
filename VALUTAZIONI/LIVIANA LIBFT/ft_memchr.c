/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:02:08 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/12 10:28:12 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	void	*result;

	result = NULL;
	while (n > 0)
	{
		if (*(unsigned char *) s == (unsigned char) c)
		{
			result = (unsigned char *)s;
			return (result);
		}
		s++;
		n--;
	}
	return (result);
}

/*int main()
{
	char	array[] = "ehi ciao";

	printf("%s", ft_memchr(array, 'i', sizeof(array)));
}*/
