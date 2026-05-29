/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 23:22:28 by faiello           #+#    #+#             */
/*   Updated: 2025/10/16 23:24:30 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

static char	*free_and_null(char *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

static char	*read_into_stash(int fd, char *stash)
{
	char	buffer[BUFFER_SIZE + 1];
	ssize_t	bytes;
	char	*tmp;

	while ((!stash || !ft_strchr(stash, '\n')))
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes <= 0)
		{
			if (bytes < 0)
				return (free_and_null(stash));
			return (stash);
		}
		buffer[bytes] = '\0';
		if (stash)
			tmp = ft_strjoin(stash, buffer);
		else
			tmp = ft_strdup(buffer);
		free(stash);
		stash = tmp;
		if (!stash)
			return (NULL);
	}
	return (stash);
}

static char	*extract_line(char *stash)
{
	size_t	i;
	char	*line;

	if (!stash || !*stash)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	line = (char *)ft_calloc(i + (stash[i] == '\n') + 1, sizeof(char));
	if (!line)
		return (NULL);
	ft_memcpy(line, stash, i + (stash[i] == '\n'));
	return (line);
}

static char	*trim_stash(char *stash)
{
	size_t	i;
	char	*new_stash;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free_and_null(stash));
	new_stash = ft_strdup(stash + i + 1);
	free(stash);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[1024];
	char		*line;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	stash[fd] = read_into_stash(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = extract_line(stash[fd]);
	stash[fd] = trim_stash(stash[fd]);
	return (line);
}
