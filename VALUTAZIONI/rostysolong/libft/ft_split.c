/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:16:21 by rodolhop          #+#    #+#             */
/*   Updated: 2025/01/13 15:16:45 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(const char *s, char c)
{
	size_t	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s && *s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static char	*copy_word(const char *s, char c, size_t *idx)
{
	size_t	start;
	size_t	end;
	char	*word;

	while (s[*idx] == c)
		(*idx)++;
	start = *idx;
	while (s[*idx] && s[*idx] != c)
		(*idx)++;
	end = *idx;
	word = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!word)
		return (NULL);
	ft_memcpy(word, &s[start], (end - start));
	word[end - start] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	size_t	word_counted;
	size_t	i;
	size_t	idx;

	if (!s)
		return (NULL);
	word_counted = count_words(s, c);
	arr = (char **)malloc(sizeof(char *) * (word_counted + 1));
	if (!arr)
		return (NULL);
	i = 0;
	idx = 0;
	while (i < word_counted)
	{
		arr[i] = copy_word(s, c, &idx);
		if (!arr[i++])
		{
			return (NULL);
		}
	}
	arr[i] = NULL;
	return (arr);
}
