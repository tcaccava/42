/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dd-alo <dd-alo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 18:19:23 by dd-alo            #+#    #+#             */
/*   Updated: 2026/07/30 14:24:00 by dd-alo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isthere(char const *set, char ch)
{
	size_t	i;

	i = 0;
	while (set[i] != 0)
	{
		if (set[i] == ch)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	i;
	size_t	j;
	size_t	s1len;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	j = 0;
	s1len = ft_strlen(s1);
	while (s1[i] && isthere(set, s1[i]))
		i++;
	while (s1[i] && isthere(set, s1[s1len - 1]) && s1len > 0)
		s1len--;
	str = malloc(sizeof(char) * ((s1len - i) + 1));
	if (!str)
		return (NULL);
	while (s1[i] && i < s1len)
	{
		str[j] = s1[i];
		j++;
		i++;
	}
	str[j] = '\0';
	return (str);
}
