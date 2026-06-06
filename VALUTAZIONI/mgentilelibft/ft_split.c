/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 17:13:12 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/04 19:55:31 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free(char **result, size_t j)
{
	while (j > 0)
	{
		j--;
		free(result[j]);
	}
	free(result);
}

static size_t	ft_count_words(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static size_t	ft_word_len(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	*ft_word_dup(char const *s, char c)
{
	char	*result;
	size_t	i;

	i = 0;
	result = malloc(ft_word_len(s, c) + 1);
	if (!result)
		return (NULL);
	while (s[i] && s[i] != c)
	{
		result[i] = s[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	result = malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!result)
		return (NULL);
	while (j < ft_count_words(s, c))
	{
		while (s[i] == c)
			i++;
		result[j] = ft_word_dup(s + i, c);
		if (!result[j])
			return (ft_free(result, j), NULL);
		i += ft_word_len(s + i, c);
		j++;
	}
	result[j] = NULL;
	return (result);
}
/*int main()
{
	char	**result;
	int		i;

	// Test 1: caso normale
	printf("Test 1: 'hello,world,ciao'\n");
	result = ft_split("hello,world,ciao", ',');
	i = 0;
	while (result[i])
	{
		printf("  [i ++;%d] = %s\n", i, result[i]);
		i++;
	}

	// Test 2: delimitatori multipli
	printf("\nTest 2: 'hello,,,,world'\n");
	result = ft_split("hello,,,,world", ',');
	i = 0;
	while (result[i])
	{
		printf("  [%d] = %s\n", i, result[i]);
		i++;
	}

	// Test 3: delimitatori all'inizio e alla fine
	printf("\nTest 3: ',,,hello,world,,,'\n");
	result = ft_split(",,,hello,world,,,", ',');
	i = 0;
	while (result[i])
	{
		printf("  [%d] = %s\n", i, result[i]);
		i++;
	}

	// Test 4: stringa vuota
	printf("\nTest 4: stringa vuota ''\n");
	result = ft_split("", ',');
	i = 0;
	while (result[i])
	{
		printf("  [%d] = %s\n", i, result[i]);
		i++;
	}
	printf("  (nessuna parola)\n");

	return (0);
}*/
