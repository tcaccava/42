/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrunial <ibrunial@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 08:33:56 by ibrunial          #+#    #+#             */
/*   Updated: 2024/12/16 13:44:32 by ibrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*dest;
	int		len;

	while (*s1 && is_in_set(*s1, set))
		s1++;
	len = ft_strlen(s1);
	if (len == 0)
		return ((char *)ft_calloc(1, 1));
	while (is_in_set(s1[len - 1], set))
		len--;
	dest = (char *)malloc(sizeof(char) * len + 1);
	if (!dest)
		return (NULL);
	dest[len--] = '\0';
	while (len >= 0)
	{
		dest[len] = s1[len];
		len--;
	}
	return (dest);
}

// int main()
// {
// 	printf("(%s)\n", ft_strtrim("prova sono qui allora prova", "prova"));
// }
