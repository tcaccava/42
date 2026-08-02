/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:39:35 by keiestre          #+#    #+#             */
/*   Updated: 2026/07/06 20:12:56 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count(char *str, char c)
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

static int	splitter(char **dest, char *src, char c)
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
			while (src[i] != '\0' && src[i] != c)
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

char	**ft_split(const char *s, char c)
{
	char	**res;
	char	*str;
	int		words;
	int		ciao;

	str = (char *)s;
	words = count(str, c);
	res = (char **)malloc((words + 1) * sizeof(char *));
	if (res == NULL)
		return (NULL);
	ciao = splitter(res, str, c);
	if (ciao != 0)
	{
		while (ciao >= 0)
		{
			free(res[ciao]);
			ciao--;
		}
		free(res);
		return (NULL);
	}
	res[words] = NULL;
	return (res);
}
