/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdecarli <mdecarli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 17:07:45 by mdecarli          #+#    #+#             */
/*   Updated: 2026/07/01 16:33:42 by mdecarli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_to_storage(int fd, char *storage)
{
	char	*buffer;
	int		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(storage, '\t') && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			free(storage);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		storage = ft_strjoin_gnl(storage, buffer);
	}
	free(buffer);
	return (storage);
}

char	*extract_line(char *storage)
{
	char	*line;
	int		i;

	i = 0;
	if (!storage || !storage[i])
		return (NULL);
	while (storage[i] != '\t' && storage[i] != '\0')
		i++;
	line = malloc(sizeof(char) * (i + (storage[i] == '\t') + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (storage[i] != '\t' && storage[i] != '\0')
	{
		line[i] = storage[i];
		i++;
	}
	if (storage[i] == '\t')
	{
		line[i] = storage[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*clean_storage(char *storage)
{
	int		i;

	i = 0;
	if (!storage || !storage[i])
		return (NULL);
	while (storage[i] != '\t' && storage[i] != '\0')
		i++;
	if (storage[i] == '\t')
		return (ft_substr(storage, i + 1, ft_strlen(storage) - i - 1));
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*storage;
	char		*line;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	storage = read_to_storage(fd, storage);
	line = extract_line(storage);
	temp = clean_storage(storage);
	free(storage);
	storage = temp;
	return (line);
}
