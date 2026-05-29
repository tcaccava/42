/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_lines_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 23:28:07 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/26 23:30:24 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static char	*init_line_buffer(int *capacity)
{
	char	*line;

	*capacity = 64;
	line = malloc(*capacity);
	if (!line)
		return (NULL);
	return (line);
}

static char	*expand_line_buffer(char *line, int len, int *capacity)
{
	char	*temp;
	int		i;

	*capacity *= 2;
	temp = malloc(*capacity);
	if (!temp)
	{
		free(line);
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		temp[i] = line[i];
		i++;
	}
	free(line);
	return (temp);
}

static int	append_char_to_line(char **line, char c, int *len, int *capacity)
{
	if (*len >= *capacity - 1)
	{
		*line = expand_line_buffer(*line, *len, capacity);
		if (!*line)
			return (0);
	}
	(*line)[*len] = c;
	(*len)++;
	return (1);
}

char	*read_single_line(int fd)
{
	char	*line;
	char	buffer[1];
	int		bytes_read;
	int		len;
	int		capacity;

	line = init_line_buffer(&capacity);
	if (!line)
		return (NULL);
	len = 0;
	bytes_read = read(fd, buffer, 1);
	while (bytes_read > 0 && buffer[0] != '\n')
	{
		if (!append_char_to_line(&line, buffer[0], &len, &capacity))
			return (NULL);
		bytes_read = read(fd, buffer, 1);
	}
	if (len == 0 && bytes_read == 0)
	{
		free(line);
		return (NULL);
	}
	line[len] = '\0';
	return (line);
}
