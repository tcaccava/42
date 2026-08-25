/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoda <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 16:40:13 by dmoda             #+#    #+#             */
/*   Updated: 2026/07/06 14:53:45 by dmoda            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_store_clean(int fd, char *str)
{
	char	*buffer;
	char	*old;
	int		bytes;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes = 1;
	while ((!str || !ft_strchr(str, '\n')) && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (free(buffer), free(str), NULL);
		buffer[bytes] = '\0';
		old = str;
		str = ft_strjoin(str, buffer);
		if (!str)
			return (free(buffer), free(old), NULL);
		free(old);
	}
	free(buffer);
	if (!str || str[0] == '\0')
		return (free(str), NULL);
	return (str);
}

char	*get_that_line(char *str)
{
	char	*line;
	int		i;

	i = 0;
	if (!str[i])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		line[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*rest;
	char		*new_rest;
	int			i;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	rest = read_store_clean(fd, rest);
	if (!rest)
		return (NULL);
	line = get_that_line(rest);
	if (!line)
		return (NULL);
	i = 0;
	while (rest[i] && rest[i] != '\n')
		i++;
	if (rest[i] == '\n')
		i++;
	new_rest = ft_strdup(rest + i);
	free(rest);
	rest = new_rest;
	return (line);
}
