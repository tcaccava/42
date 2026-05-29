/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscarcin <pscarcin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:34:30 by pscarcin          #+#    #+#             */
/*   Updated: 2025/10/31 14:35:36 by pscarcin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

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
	static char	*buffers[OPEN_MAX];

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		return (NULL);
	}
	buffers[fd] = read_and_join(fd, buffers[fd]);
	if (!buffers[fd])
		return (NULL);
	line = extract_line(&buffers[fd]);
	return (line);
}

/*
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int fd1;
	int fd2;
	char *line;

	fd1 = open("file1.txt", O_RDONLY);
	fd2 = open("file2.txt", O_RDONLY);

	printf(">>> Leggo una riga dal file 1\n");
	line = get_next_line(fd1);
	printf("%s", line);
	free(line);

	printf(">>> Leggo una riga dal file 2\n");
	line = get_next_line(fd2);
	printf("%s", line);
	free(line);

	printf(">>> Leggo un’altra riga dal file 1\n");
	line = get_next_line(fd1);
	printf("%s", line);
	free(line);

	printf(">>> Leggo tutto il resto dal file 2\n");
	while ((line = get_next_line(fd2)) != NULL)
	{
		printf("%s", line);
		free(line);
	}

	close(fd1);
	close(fd2);
	return (0);
}*/