/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 17:44:22 by fionni            #+#    #+#             */
/*   Updated: 2025/01/02 00:00:00 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_split(char **ptr, size_t size)
{
	while (size > 0)
	{
		size--;
		free(ptr[size]);
	}
	free(ptr);
}

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s == c)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static char	*extract_word(char const **s, char c)
{
	size_t		len;
	char const	*start;
	char		*word;

	while (**s && **s == c)
		(*s)++;
	start = *s;
	while (**s && **s != c)
		(*s)++;
	len = (size_t)(*s - start);
	word = (char *)malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	ft_memcpy(word, start, len);
	word[len] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	size_t	total;
	size_t	i;
	char	**out;

	i = 0;
	if (!s)
		return (NULL);
	total = count_words(s, c);
	out = (char **)malloc((total + 1) * sizeof(char *));
	if (!out)
		return (NULL);
	while (i < total)
	{
		out[i] = extract_word(&s, c);
		if (!out[i])
		{
			free_split(out, i);
			return (NULL);
		}
		i++;
	}
	out[i] = NULL;
	return (out);
}
