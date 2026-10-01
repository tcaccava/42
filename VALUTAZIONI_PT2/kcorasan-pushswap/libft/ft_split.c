/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slorefic <slorefic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 12:24:44 by slorefic          #+#    #+#             */
/*   Updated: 2026/05/28 12:27:28 by slorefic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static int	count_words(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static void	free_all(char **res, int i)
{
	while (i--)
		free(res[i]);
	free(res);
}

static char	*get_word(char const *s, char c, int *i)
{
	char	*word;
	int		len;
	int		j;

	len = 0;
	while (s[*i + len] && s[*i + len] != c)
		len++;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	j = 0;
	while (j < len)
		word[j++] = s[(*i)++];
	word[j] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		i;
	int		w;

	if (!s)
		return (NULL);
	res = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!res)
		return (NULL);
	i = 0;
	w = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			res[w] = get_word(s, c, &i);
			if (!res[w++])
				return (free_all(res, w - 1), NULL);
		}
	}
	res[w] = NULL;
	return (res);
}

/*int	main(void)
{
		char	**tab;
		int		i;

		tab = ft_split("salve a tutti ragazzi, io odio questa funzione", ' ');
		if (!tab)
				return (1);

		i = 0;
		while (tab[i])
		{
				printf("%s\n", tab[i]);
				free(tab[i]);
				i++;
		}
		free(tab);

		return (0);
}*/