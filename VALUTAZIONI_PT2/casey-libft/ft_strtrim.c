/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 18:20:54 by keiestre          #+#    #+#             */
/*   Updated: 2026/09/23 18:20:55 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	checker(char s, char const *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == s)
			return (1);
		i++;
	}
	return (0);
}

static	char	*writer(char *dest, char const *src, int start, int end)
{
	int	i;

	i = 0;
	while (start < end)
	{
		dest[i] = src[start];
		i++;
		start++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*trimd;

	start = 0;
	end = 0;
	if (s1 == NULL || set == NULL)
		return (NULL);
	while (s1[end] != '\0')
		end++;
	while (s1[start] != '\0' && checker(s1[start], set))
		start++;
	while (end > start && checker(s1[end -1], set))
		end--;
	trimd = (char *)malloc((end - start + 1) * sizeof(char));
	if (trimd == NULL)
		return (NULL);
	writer(trimd, s1, start, end);
	return (trimd);
}
