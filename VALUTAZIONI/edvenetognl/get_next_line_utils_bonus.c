/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 05:26:37 by edveneto          #+#    #+#             */
/*   Updated: 2026/07/08 05:51:13 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

int	ft_charcheck(char *stash, char c)
{
	int	i;

	i = 0;
	if (!stash)
		return (0);
	while (stash[i])
	{
		if (stash[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strjoin(char *stash, char *buffer)
{
	int		i;
	int		j;
	char	*res;

	res = malloc(sizeof(char) * (ft_strlen(stash) + ft_strlen(buffer) + 1));
	if (!res)
	{
		free(stash);
		return (NULL);
	}
	i = -1;
	if (stash)
		while (stash[++i])
			res[i] = stash[i];
	else
		i = 0;
	j = -1;
	while (buffer[++j])
		res[i + j] = buffer[j];
	res[i + j] = '\0';
	free(stash);
	return (res);
}
