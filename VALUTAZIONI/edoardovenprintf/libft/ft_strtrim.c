/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:37:21 by edveneto          #+#    #+#             */
/*   Updated: 2026/05/28 14:37:21 by edveneto         ###   ########.fr       */
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
	size_t	len;
	char	*trimmed;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	while (s1[i] && is_set(s1[i], set))
		i++;
	len = ft_strlen(s1);
	if (i == len)
		return (ft_strdup(""));
	while (len > i && is_set(s1[len - 1], set))
		len--;
	trimmed = (char *)malloc(sizeof(char) * (len - i + 1));
	if (!trimmed)
		return (NULL);
	ft_strlcpy(trimmed, &s1[i], len - i + 1);
	return (trimmed);
}
