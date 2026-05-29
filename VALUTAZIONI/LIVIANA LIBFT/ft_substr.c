/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:01:29 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/14 18:19:54 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substring;
	size_t	i;

	while (*s && start > 0)
	{
		s += 1;
		start--;
	}
	i = ft_strlen(s);
	if (i > len)
		substring = (char *) malloc ((len + 1) * (sizeof(char)));
	else
		substring = (char *) malloc ((i + 1) * (sizeof(char)));
	if (substring == NULL)
		return (NULL);
	ft_strlcpy(substring, s, len + 1);
	return (substring);
}

/*#include <stdio.h>

int main()
{
	char	s[] = "tripouille";
	printf("%s", ft_substr(s, 1, 1));
}*/
