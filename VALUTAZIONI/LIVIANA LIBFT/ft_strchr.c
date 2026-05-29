/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:42:52 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/12 17:38:08 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*result;
	char	a;

	result = (char *)s;
	a = c;
	while (*result)
	{
		if (*result == a)
			return (result);
		result++;
	}
	if (*result == a)
		return (result);
	return (NULL);
}

/*#include <stdio.h>

int main(int ac, char **av)
{
	printf("%s", ft_strchr("tkdeda", 372));
	printf("%d", 't' + 256);
}*/
