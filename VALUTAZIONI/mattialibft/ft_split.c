/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtozzi-b <mtozzi-b@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 18:14:46 by mtozzi-b          #+#    #+#             */
/*   Updated: 2025/01/18 18:19:45 by mtozzi-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_countword(char const *s, char c)
{
	size_t	count;

	if (!*s)
		return (0);
	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

static char	**ft_free_split(char **lst, int i)
{
	while (i >= 0)
		free(lst[i--]);
	free(lst);
	return (NULL);
}

static char	*get_next_word(char const **s, char c)
{
	size_t	len;
	char	*word;

	while (**s == c)
		(*s)++;
	len = 0;
	while ((*s)[len] && (*s)[len] != c)
		len++;
	word = ft_substr(*s, 0, len);
	*s += len;
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**lst;
	int		i;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = ft_countword(s, c);
	lst = malloc((word_count + 1) * sizeof(char *));
	if (!lst)
		return (NULL);
	i = 0;
	while (i < (int)word_count)
	{
		lst[i] = get_next_word(&s, c);
		if (!lst[i])
			return (ft_free_split(lst, i - 1));
		i++;
	}
	lst[i] = NULL;
	return (lst);
}
