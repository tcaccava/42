/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 18:19:30 by keiestre          #+#    #+#             */
/*   Updated: 2026/09/23 18:19:31 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	wordcounter(char *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] == c)
			i++;
		if (str[i] != '\0')
		{
			count++;
			while (str[i] != '\0' && str[i] != c)
				i++;
		}
	}
	return (count);
}

static	int	splitter(char **dest, char *src, char c)
{
	int	i;
	int	index;
	int	start;
	int	len;

	i = 0;
	index = 0;
	while (src[i] != '\0')
	{
		while (src[i] == c && src[i] != '\0')
			i++;
		if (src[i] != '\0')
		{
			start = i;
			while (src[i] != c && src[i] != '\0')
				i++;
			len = i - start;
			dest[index] = (char *)malloc((len + 1) * sizeof(char));
			if (dest[index] == NULL)
				return (index);
			ft_strlcpy(dest[index], src + start, len + 1);
			index++;
		}
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	char	*str;
	int		words;
	int		allocated;

	if (s == NULL)
		return (NULL);
	str = (char *)s;
	words = wordcounter(str, c);
	split = (char **)malloc((words + 1) * sizeof(char *));
	if (split == NULL)
		return (NULL);
	allocated = splitter(split, str, c);
	if (allocated != 0)
	{
		while (allocated >= 0)
		{
			free(split[allocated]);
			allocated--;
		}
		free(split);
		return (NULL);
	}
	split[words] = NULL;
	return (split);
}
