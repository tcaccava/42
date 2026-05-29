/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:22:03 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/13 17:28:44 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ischar(const char c, const char *s)
{
	while (*s)
	{
		if (c == *s)
			return (0);
		s++;
	}
	return (1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int			len;
	const char	*end;
	char		*str;

	len = ft_strlen(s1);
	end = &s1[len - 1];
	while (ischar(*s1, set) == 0)
	{
		s1++;
		len--;
	}
	while (ischar(*end, set) == 0)
	{
		end--;
		len--;
	}
	str = ft_substr(s1, 0, len);
	return (str);
}

/*#include <stdio.h>

int main(int ac, char **av)
{
	printf("%s", ft_strtrim("   xxxtripouille   xxx", " x"));
}*/
