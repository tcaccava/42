/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnlutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:22:55 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/27 19:23:00 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Calcs string length
size_t ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

// Dups string
char *ft_strdup(const char *s)
{
	char *dup;
	size_t i;

	i = 0;
	if (!s)
		return (NULL);
	dup = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!dup)
		return (NULL);
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

// Joings strings
char *ft_strjoin(char *s1, char *s2)
{
	char *join;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	join = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!join)
		return (NULL);
	ft_strcpy(join, s1);
	ft_strcpy(join + ft_strlen(s1), s2);
	return (join);
}

// Search the first occurrence of a char in a string
char *ft_strchr(const char *s, int c)
{
	if (!s)
		return (0);
	while (*s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (0);
}

// Copies a string
char *ft_strcpy(char *dest, const char *src)
{
	char *original_dest;

	original_dest = dest;
	if (!src)
		return (NULL);
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (original_dest);
}
