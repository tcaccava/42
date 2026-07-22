/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:25:00 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/22 12:16:30 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
/*#include <stdio.h>*/

int	splitstrlen(char const *s, char c, int i)
{
	int	j;

	j = 0;
	while (s[i] != '\0' && s[i] != c)
	{
		j++;
		i++;
	}
	return (j);
}

int	splitamount(char const *s, char c)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (s[i] != '\0')
	{
		if ((i == 0 || s[i - 1] == c) && s[i] != c)
			n++;
		i++;
	}
	return (n);
}

char	*splitter(char	const *s, char c, int n)
{
	char	*string;
	int		j;

	j = 0;
	string = malloc(sizeof(char) * (splitstrlen(s, c, n) + 1));
	if (!string)
		return (NULL);
	while (s[n] != c && s[n] != '\0')
		string[j++] = s[n++];
	string[j] = '\0';
	return (string);
}

void	freesplitter(char **splitted, int i)
{
	while (i > 0)
	{
		i--;
		free(splitted[i]);
	}
	free(splitted);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		n;
	char	**splitted;

	i = 0;
	n = 0;
	splitted = malloc(sizeof(char *) * (splitamount(s, c) + 1));
	if (!splitted)
		return (NULL);
	while (s[n] != '\0')
	{
		while (s[n] == c)
			n++;
		if (s[n] == '\0')
			break ;
		splitted[i] = splitter(s, c, n);
		if (!splitted[i])
			return (freesplitter(splitted, i), NULL);
		while (s[n] != c && s[n] != '\0')
			n++;
		i++;
	}
	splitted[i] = (NULL);
	return (splitted);
}
