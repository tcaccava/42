/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 20:23:48 by ggiument          #+#    #+#             */
/*   Updated: 2026/06/05 19:30:36 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	**count_and_matrix(char const *s, char c)

{
	size_t	i;
	size_t	words;
	char	**matrix;

	i = 0;
	words = 0;
	while (s[i])
	{
		if (i == 0 && s[i] != c)
			words++;
		else if (i > 0 && s[i - 1] == c && s[i] != c)
			words++;
		i++;
	}
	matrix = malloc((words + 1) * sizeof(char *));
	if (!matrix)
		return (NULL);
	return (matrix);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	start;
	size_t	u;
	char	**matrix;

	i = 0;
	start = 0;
	u = 0;
	matrix = count_and_matrix(s, c);
	while (s[i])
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] != c && s[i])
			i++;
		if (i != start)
		{
			matrix[u] = ft_substr(s, start, i - start);
			if (!matrix[u++])
				return (NULL);
		}
	}
	matrix[u] = NULL;
	return (matrix);
}
/*
int	main(int c, char **v)
{
char	**str;
int		i;

if (c < 2)
return (0);
str = ft_split(v[1], 'a');
i = 0;
while (str && str[i])
{
printf("%s\n", str[i]);
i++;
}
free(str);
return (0);
}*/
