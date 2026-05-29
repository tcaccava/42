/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 18:51:06 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/12 17:41:51 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char		*p1;
	char		*p2;
	size_t		j;
	size_t		i;

	j = 0;
	p1 = (char *) big;
	if (*little == '\0')
		return (p1);
	if (len == 0)
		return (NULL);
	while (*p1 && j < len)
	{
		p2 = (char *) little;
		i = 0;
		while (p1[i] == p2[i] && j + i < len)
		{
			i++;
			if (p2[i] == '\0')
				return (p1);
		}
		p1++;
		j++;
	}
	return (NULL);
}

/*#include <string.h>
#include <stdio.h>

int main()
{
	//printf("%s", ft_strnstr("come ti chiami", "ch", -1));
	printf("%s\n", ft_strnstr("aaabcabcd", "cd", 8));
	//printf("%s", strnstr("come ti chiami", "ch", -1));
}*/
