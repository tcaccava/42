/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smattei <smattei@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:29:22 by smattei           #+#    #+#             */
/*   Updated: 2025/07/31 14:51:16 by smattei          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*strjoin_lines(char *all_lines, char *buffer)
{
	char	*result;

	if (!all_lines)
		return (ft_strdup(buffer));
	result = ft_strjoin(all_lines, buffer);
	if (!result)
	{
		free(all_lines);
		return (NULL);
	}
	free(all_lines);
	return (result);
}

static char	*extract_line(char **all_lines)
{
	int		i;
	char	*line;
	char	*remainder;

	i = 0;
	while ((*all_lines)[i] != '\n' && (*all_lines)[i] != '\0')
		i++;
	line = malloc((i + 2) * sizeof(char));
	if (!line)
	{
		free(*all_lines);
		return (NULL);
	}
	ft_strlcpy(line, *all_lines, i + 2);
	if ((*all_lines)[i] == '\n')
		remainder = ft_strdup(&(*all_lines)[i + 1]);
	else
		remainder = NULL;
	free(*all_lines);
	*all_lines = remainder;
	return (line);
}

char	*get_next_line(int fd)
{
	ssize_t		bytes_read;
	char		*buffer;
	static char	*all_lines [OPEN_MAX];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		all_lines[fd] = strjoin_lines(all_lines, buffer);
		if (ft_strchr(all_lines, '\n'))
		{
			free(buffer);
			return (extract_line(&all_lines));
		}
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (all_lines && *all_lines[fd] != '\0')
		return (extract_line(&all_lines));
	return ((free(all_lines), all_lines = NULL));
}
