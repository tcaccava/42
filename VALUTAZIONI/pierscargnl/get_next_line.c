/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscarcin <pscarcin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:33:48 by pscarcin          #+#    #+#             */
/*   Updated: 2025/10/31 14:36:26 by pscarcin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_and_join(int fd, char *buffer)
{
	char	*read_buf;
	ssize_t	bytes_read;
	char	*tmp;

	read_buf = malloc(BUFFER_SIZE + 1);
	if (!read_buf)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(buffer, '\n'))
	{
		bytes_read = read(fd, read_buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(read_buf);
			free(buffer);
			return (NULL);
		}
		read_buf[bytes_read] = '\0';
		tmp = ft_strjoin(buffer, read_buf);
		free(buffer);
		buffer = tmp;
	}
	free(read_buf);
	return (buffer);
}

char	*extract_line(char **buffer)
{
	char	*line;
	char	*remainder;
	size_t	i;

	i = 0;
	if (!*buffer || !**buffer)
	{
		free(*buffer);
		*buffer = NULL;
		return (NULL);
	}
	while ((*buffer)[i] && (*buffer)[i] != '\n')
		i++;
	line = (char *)malloc((i + 2) * sizeof(char));
	if (!line)
		return (NULL);
	ft_strlcpy(line, *buffer, i + 2);
	remainder = ft_strdup(*buffer + i + ((*buffer)[i] == '\n'));
	free(*buffer);
	*buffer = remainder;
	return (line);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buffer;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		return (NULL);
	}
	buffer = read_and_join(fd, buffer);
	if (!buffer)
		return (NULL);
	line = extract_line(&buffer);
	return (line);
}


#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd;
	char	*line;
	int		i;

	fd = open("file1.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (1);
	}
	i = 1;
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("Line %d: %s", i, line);
		free(line);
		i++;
	}
	close(fd);
	return (0);
}
