/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdecarli <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 17:12:39 by mdecarli          #+#    #+#             */
/*   Updated: 2026/05/28 19:04:01 by mdecarli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len);

static int	count_words(const char *s, char c)
{
	int	count;
	int	word;

	count = 0;
	word = 0;
	while (*s)
	{
		if (*s != c && word == 0)
		{
			word = 1;
			count++;
		}
		else if (*s == c)
			word = 0;
		s++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	word_len;
	int		i;

	if (!s)
		return (NULL);
	res = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s == '\0')
			break ;
		word_len = 0;
		while (s[word_len] && s[word_len] != c)
			word_len++;
		res[i++] = ft_substr(s, 0, word_len);
		s += word_len;
	}
	res[i] = NULL;
	return (res);
}
/*int	main(void)
{
	char	*str = "Miao.42.Ciao";
	char	**words;
	int	i;

	words = ft_split(str, '.');
	if (!words)
		return (1);
	i = 0;
	while (words[i] != NULL)
	{
		printf("%d: %s\n", i, words[i]);
		free(words[i]);
		i++;
	}
	free(words);
	return (0);
}*/
