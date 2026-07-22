/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:21:32 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/16 14:53:08 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*stashpointer(char *stash)
{
	int		i;
	int		n;
	char	*newstash;

	i = 0;
	n = 0;
	if (!stash)
		return (NULL);
	while (stash[i] != '\n')
		i++;
	i++;
	n = readlen(stash + i);
	if (n == 0)
		return (NULL);
	newstash = malloc(n + 1);
	if (!newstash)
		return (NULL);
	n = 0;
	while (stash[i + n] != '\0')
	{
		newstash[n] = stash[i + n];
		n++;
	}
	newstash[n] = '\0';
	return (newstash);
}

char	*liner(char *stash)
{
	int		i;
	char	*line;

	i = 0;
	while (stash[i] != '\n')
		i++;
	line = malloc(i + 2);
	i = 0;
	while (stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\n';
	line[i + 1] = '\0';
	return (line);
}

char	*stashmaker(char *stash, char *buffer, int fd)
{
	int	bytes;

	if (nlfinder(stash) == 1)
		return (stash);
	bytes = read(fd, buffer, BUFFER_SIZE);
	if (bytes <= 0)
		return (stash);
	buffer[bytes] = '\0';
	stash = readjoin(stash, buffer);
	while (nlfinder(stash) == 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes <= 0)
			break ;
		buffer[bytes] = '\0';
		stash = readjoin(stash, buffer);
	}
	if (!stash)
		return (NULL);
	else
		return (stash);
}

char	*freethebuffer(char *buffer, char *returned)
{
	free(buffer);
	if (!returned)
		return (NULL);
	return (returned);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	static char	*stash;
	char		*tmp;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	stash = stashmaker(stash, buffer, fd);
	if (nlfinder(stash) == 0)
	{
		line = stash;
		stash = NULL;
		return (freethebuffer(buffer, line));
	}
	line = liner(stash);
	tmp = stash;
	stash = stashpointer(stash);
	free (tmp);
	return (freethebuffer(buffer, line));
}
