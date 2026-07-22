/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:21:50 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/10 16:56:06 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	readlen(char *s)
{
	int	i;

	i = 0;
	if (s)
	{
		while (s[i])
			i++;
		return (i);
	}
	else
		return (0);
}

char	*readjoin(char *s1, char *s2)
{
	int		i;
	int		n;
	char	*s3;

	s3 = malloc((readlen(s1) + readlen(s2)) + 1);
	if (!s3)
		return (NULL);
	i = 0;
	n = 0;
	if (s1)
	{
		while (s1[i])
		{
			s3[i] = s1[i];
			i++;
		}
	}
	while (s2[n])
		s3[i++] = s2[n++];
	s3[i] = '\0';
	free (s1);
	return (s3);
}

int	nlfinder(char *s)
{
	int	i;

	i = 0;
	if (s)
	{
		while (s[i])
		{
			if (s[i] == '\n')
				return (1);
			i++;
		}
	}
	return (0);
}
