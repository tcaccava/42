/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 23:21:12 by faiello           #+#    #+#             */
/*   Updated: 2025/10/16 23:21:27 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_parts(char const *s, char c)
{
	size_t	count;
	int		in_part;

	count = 0;
	in_part = 0;
	while (*s)
	{
		if (*s != c && !in_part)
		{
			in_part = 1;
			count++;
		}
		else if (*s == c)
			in_part = 0;
		s++;
	}
	return (count);
}

static char	*dup_part(char const *s, size_t start, size_t len)
{
	char	*out;

	out = (char *)ft_calloc(len + 1, sizeof(char));
	if (!out)
		return (NULL);
	ft_memcpy(out, s + start, len);
	out[len] = '\0';
	return (out);
}

static void	*cleanup(char **parts, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(parts[i]);
		i++;
	}
	free(parts);
	return (NULL);
}

static int	fill_parts(char **parts, char const *s, char c)
{
	size_t	idx;
	size_t	start;
	size_t	len;

	idx = 0;
	start = 0;
	while (s[start])
	{
		while (s[start] && s[start] == c)
			start++;
		if (!s[start])
			break ;
		len = 0;
		while (s[start + len] && s[start + len] != c)
			len++;
		parts[idx] = dup_part(s, start, len);
		if (!parts[idx])
			return ((int)(cleanup(parts, idx) == NULL));
		idx++;
		start += len;
	}
	parts[idx] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**parts;

	if (!s)
		return (NULL);
	parts = (char **)ft_calloc(count_parts(s, c) + 1, sizeof(char *));
	if (!parts)
		return (NULL);
	if (!fill_parts(parts, s, c))
		return (NULL);
	return (parts);
}
