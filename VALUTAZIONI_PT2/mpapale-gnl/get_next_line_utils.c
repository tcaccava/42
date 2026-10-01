/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 12:07:36 by mpapale           #+#    #+#             */
/*   Updated: 2026/09/27 16:05:01 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	b;

	if (!str)
		return (0);
	b = 0;
	while (*str)
	{
		b = b + 1;
		str++;
	}
	return (b);
}

char	*ft_strchr(const char *str, int to_find)
{
	if (!str)
		return (NULL);
	while (*str)
	{
		if (*str == (char)to_find)
			return ((char *)str);
		str++;
	}
	if (*str == (char)to_find)
		return ((char *)str);
	return (NULL);
}

char	*ft_strjoin_gnl(char const *s1, char const *s2)
{
	char	*join;
	size_t	i;
	size_t	j;

	if (!s1)
		return (ft_strdup(s2));
	join = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!join)
	{
		free((char *)s1);
		return (NULL);
	}
	i = 0;
	while (s1[i])
	{
		join[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		join[i++] = s2[j++];
	join[i] = '\0';
	free((char *)s1);
	return (join);
}

char	*ft_strdup(const char *src)
{
	size_t	i;
	char	*array;

	if (!src)
		return (NULL);
	array = malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (src[i])
	{
		array[i] = src[i];
		i++;
	}
	array[i] = '\0';
	return (array);
}
