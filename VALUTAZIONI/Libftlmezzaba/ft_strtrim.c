/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:43:16 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/05/20 15:09:47 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	char	*trimmed;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	while (s1[i] && is_set(s1[i], set))
		i++;
	j = ft_strlen(s1);
	while (j > i && is_set(s1[j - 1], set))
		j--;
	trimmed = (char *)malloc(sizeof(char) * ((j - i) + 1));
	if (!trimmed)
		return (NULL);
	ft_memcpy(trimmed, s1 + i, j - i);
	trimmed[j - i] = '\0';
	return (trimmed);
}
