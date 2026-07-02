/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grocca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 16:45:14 by grocca            #+#    #+#             */
/*   Updated: 2026/07/01 16:45:20 by grocca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*substruntil(char *s, int len)
{
	char	*p;

	p = malloc((len + 1) * sizeof(char));
	if (!p)
		return (NULL);
	p[len] = '\0';
	while (len--)
		p[len] = s[len];
	free(s);
	return (p);
}

void	strshift(char *s, int j)
{
	int	i;

	i = 0;
	while (s[j + i])
	{
		s[i] = s[j + i];
		i++;
	}
	while (s[i])
	{
		s[i] = '\0';
		i++;
	}
}

size_t	findchr(char *s, char c)
{
	size_t	i;

	if (!s)
		return (0);
	i = 1;
	while (s[i - 1])
	{
		if (s[i - 1] == c)
			return (i);
		i++;
	}
	if (!c)
		return (i - 1);
	return (0);
}

char	*join(char *s1, char *s2)
{
	char	*p;
	int		i;
	int		j;

	i = 0;
	j = 0;
	p = malloc((findchr(s1, '\0') + findchr(s2, '\0') + 1) * sizeof(char));
	if (!p)
		return (NULL);
	while (s1 && s1[i])
	{
		p[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		p[i] = s2[j];
		i++;
		j++;
	}
	p[i] = '\0';
	if (s1)
		free(s1);
	return (p);
}
