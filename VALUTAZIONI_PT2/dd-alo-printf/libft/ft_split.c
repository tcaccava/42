/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dd-alo <dd-alo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 16:31:28 by dd-alo            #+#    #+#             */
/*   Updated: 2026/07/30 16:33:38 by dd-alo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	wrd_counter(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (count);
}

static void	free_all(char **array, size_t j)
{
	while (j > 0)
	{
		j--;
		free(array[j]);
	}
	free(array);
}

static size_t	wrd_len(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

char	**ft_split(char const *s, char c)
{
	size_t	j;
	char	**array;

	if (!s)
		return (NULL);
	array = malloc(sizeof(char *) * (wrd_counter(s, c) + 1));
	if (!array)
		return (NULL);
	j = 0;
	while (*s)
	{
		if (*s != c)
		{
			array[j] = ft_substr(s, 0, wrd_len(s, c));
			if (!array[j++])
				return (free_all(array, j - 1), NULL);
			s += wrd_len(s, c);
		}
		else
			s++;
	}
	array[j] = NULL;
	return (array);
}
