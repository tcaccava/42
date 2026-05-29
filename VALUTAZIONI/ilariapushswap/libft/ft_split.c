/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 20:39:10 by ilnassi           #+#    #+#             */
/*   Updated: 2024/12/23 12:10:05 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_charset(char const c, char sep)
{
	if (c == sep)
		return (1);
	return (0);
}

static int	ft_wordcount(char const *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && ft_charset(s[i], c))
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && !ft_charset(s[i], c))
				i++;
		}
	}
	return (count);
}

static char	*ft_sub_cpy(char const *s, int first, int end)
{
	char	*word;

	word = ft_substr(s, first, end - first);
	if (!word)
		return (NULL);
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		start;
	int		count;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	count = ft_wordcount(s, c);
	res = (char **)malloc(sizeof(char *) * (count + 1));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (j < count)
	{
		while (s[i] && ft_charset(s[i], c))
			i++;
		start = i;
		while (s[i] && !ft_charset(s[i], c))
			i++;
		res[j++] = ft_sub_cpy(s, start, i);
	}
	res[j] = NULL;
	return (res);
}
/*
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *str = "   Hello  world   42   !!   ";
    char **split_str = split(str, ' ');

    if (split_str)
    {
        for (int i = 0; split_str[i] != NULL; i++)
        {
            printf("Word %d: '%s'\n", i, split_str[i]);
            free(split_str[i]);  // Libera la memoria di ciascuna parola
        }
        free(split_str);  // Libera l'array principale
    }

    return 0;
}
*/
