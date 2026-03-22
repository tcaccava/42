/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:18:13 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/27 19:18:24 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	*get_next_line(int fd)
{
	static char	*loot;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (!loot || !ft_strchr(loot, '\n'))
	{
		if (!read_join(fd, &loot))
			break ;
	}
	if (!loot)
		return (NULL);
	line = extract_line(loot);
	loot = clear_loot(loot);
	return (line);
}

int	read_join(int fd, char **loot)
{
	char	*buffer;
	char	*temp;
	ssize_t	bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (0);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read <= 0)
	{
		free(buffer);
		return (0);
	}
	buffer[bytes_read] = '\0';
	temp = *loot;
	*loot = ft_strjoin(temp, buffer);
	free(temp);
	free(buffer);
	if (*loot == NULL)
		return (0);
	else
		return (1);
}

char	*extract_line(char *loot)
{
	char	*line;
	size_t	i;

	i = 0;
	while (loot[i] && loot[i] != '\n')
		i++;
	if (loot[i] == '\n')
		i++;
	line = (malloc(sizeof(char) * (i + 1)));
	if (!line)
		return (NULL);
	i = 0;
	while (loot[i] && loot[i] != '\n')
	{
		line[i] = loot[i];
		i++;
	}
	if (loot[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*clear_loot(char *loot)
{
	char	*newline_pos;
	char	*new_loot;

	if (!loot)
	{
		free(loot);
		return (NULL);
	}
	newline_pos = ft_strchr(loot, '\n');
	if (!newline_pos)
	{
		free(loot);
		return (NULL);
	}
	if (*(newline_pos + 1) == '\0')
	{
		free(loot);
		return (NULL);
	}
	new_loot = ft_strdup(newline_pos + 1);
	free(loot);
	return (new_loot);
}
/*
int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
*/
