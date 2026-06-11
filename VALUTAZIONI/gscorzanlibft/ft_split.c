/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscorzon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 17:04:51 by gscorzon          #+#    #+#             */
/*   Updated: 2026/06/10 13:01:02 by gscorzon         ###   ########.fr       */
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
		if (*s != c && (*(s + 1) == c || *(s + 1) == 0))
			count++;
		s++;
	}
	return (count);
}

static int	ft_sublen(char const *s, char c, int i)
{
	int	j;
	int	l;

	j = 0;
	l = 0;
	while (i > 0)
	{
		if (j > 0 && s[j] == c && s[j - 1] != c)
			i--;
		else
			j++;
	}
	if (j > 0 && s[j] != c && s[j - 1] == c)
		i--;
	while (s[j] == c)
		j++;
	while (s[j] != c && s[j])
	{
		j++;
		l++;
	}
	return (l);
}

static char	**ft_free_all(char **arr, size_t i)
{
	while (i > 0)
		free(arr[i--]);
	free(arr);
	return (NULL);
}

static char	**ft_arrfill(char **arr, char const *s, char c, size_t arrnum)
{
	size_t	i;
	size_t	j;
	size_t	len;

	i = 0;
	while (i < arrnum)
	{
		while (*s && *s == c)
			s++;
		len = ft_sublen(s, c, 0);
		arr[i] = (char *)malloc((len + 1) * sizeof(char));
		if (!arr[i])
			return (ft_free_all(arr, i));
		j = 0;
		while (j < len)
			arr[i][j++] = *s++;
		arr[i][j] = '\0';
		i++;
	}
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	size_t	arrnum;
	char	**arr;

	arrnum = ft_countword(s, c);
	arr = (char **)malloc(sizeof(char *) * (arrnum + 1));
	if (!arr)
		return (NULL);
	arr[arrnum] = 0;
	arr = ft_arrfill(arr, s, c, arrnum);
	return (arr);
}
