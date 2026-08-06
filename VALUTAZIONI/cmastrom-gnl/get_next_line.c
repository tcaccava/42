/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmastrom <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 12:59:23 by cmastrom          #+#    #+#             */
/*   Updated: 2026/07/29 12:59:24 by cmastrom         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*extract_line(char *stash)
{
	int		i;
	char	*line;

	if (!stash || !stash[0])
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = (char *)malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		line[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*clean_stash(char *stash)
{
	int		i;
	int		j;
	char	*new_stash;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (free(stash), NULL);
	new_stash = (char *)malloc(sizeof(char) * (ft_strlen(stash) - i + 1));
	if (!new_stash)
		return (free(stash), NULL);
	i++;
	j = 0;
	while (stash[i])
		new_stash[j++] = stash[i++];
	new_stash[j] = '\0';
	free(stash);
	return (new_stash);
}

char	*reader(int fd, char *stash)
{
	char	*readed;
	ssize_t	bytes_read;

	readed = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!readed)
		return (free(stash), NULL);
	bytes_read = 1;
	while (!has_newline(stash) && bytes_read > 0)
	{
		bytes_read = read(fd, readed, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(readed), free(stash), NULL);
		readed[bytes_read] = '\0';
		stash = ft_strjoin(stash, readed);
		if (!stash)
			return (free(readed), NULL);
	}
	free(readed);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = reader(fd, stash);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	stash = clean_stash(stash);
	return (line);
}
