/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 21:23:04 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/12/16 19:25:16 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	countlen(char const *s, char c)
{
	int		nitems;
	int		index;

	nitems = 0;
	index = 0;
	while (s[index] != c && s[index] != '\0')
	{
		nitems++;
		index++;
	}
	return (nitems);
}

void	freearray(char **array)
{
	int		n;

	n = 0;
	while (array[n] != NULL)
	{
		free(array[n]);
		n++;
	}
	free(array);
}

int	countwords(const char *s, char c)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			i++;
			words++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		else if (s[i] == c)
			i++;
	}
	return (words);
}

void	createstr(char **array, int n, int nitems, char const *s)
{
	array[n] = (char *) malloc ((nitems + 1) * (sizeof(char)));
	if (array[n] == NULL)
	{
		freearray(array);
		return ;
	}
	ft_strlcpy(array[n], s, nitems + 1);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		n;
	int		nitems;

	n = 0;
	nitems = countwords(s, c);
	array = (char **) malloc ((nitems + 1) * (sizeof(char *)));
	if (array == NULL)
		return (NULL);
	while (*s && nitems != 0)
	{
		if (*s == c)
			s++;
		else
		{
			nitems = countlen(s, c);
			createstr(array, n, nitems, s);
			if (array == NULL)
				return (NULL);
			n++;
			s += nitems;
		}
	}
	array[n] = NULL;
	return (array);
}
/*#include <stdio.h>

int	main()
{
	int		i;
	char	**array;

	i = 0;
	if (!array)
			return (0);
	array = ft_split("sdgasfgs", ' ');
	if (!array)
			return (0);
	i = 0;
	while (array[i] != NULL)
	{
		printf("%s\n", array[i]);
		i++;
	}
	free(array);
}*/
