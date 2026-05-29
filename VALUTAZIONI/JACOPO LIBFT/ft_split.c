/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrunial <ibrunial@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 09:12:20 by ibrunial          #+#    #+#             */
/*   Updated: 2024/12/13 11:04:00 by ibrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			while (*s && *s != c)
				s++;
			count++;
		}
		else
			s++;
	}
	return (count);
}

static void	free_all(char **dest, int word)
{
	while (word--)
		free(dest[word]);
	free(dest);
}

char	**ft_split(char const *s, char c)
{
	char	**dest;
	int		word;
	int		prev;
	int		now;

	now = 0;
	prev = 0;
	word = count_words(s, c);
	dest = (char **)ft_calloc(word + 1, sizeof(char *));
	word = 0;
	while (s[now])
	{
		if (s[now] != c)
		{
			while (s[now] && s[now] != c)
				now++;
			dest[word] = (char *)malloc(sizeof(char) * (now - prev + 1));
			if (dest[word] == NULL)
				return (free_all(dest, word), NULL);
			ft_strlcpy(dest[word++], &s[prev], (now-- - prev + 1));
		}
		now++;
		prev = now;
	}
	return (dest);
}

// int	main(void)
// {
// 	char **AA = ft_split("lorem ipsum", ' ');
// 	for (int i = 0; AA[i] != NULL; i++)
// 		printf("%s\n", AA[i]);
// }