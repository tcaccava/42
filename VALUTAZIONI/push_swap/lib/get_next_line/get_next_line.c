/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pspaccas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 23:37:36 by pspaccas          #+#    #+#             */
/*   Updated: 2025/04/03 00:25:14 by pspaccas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*keep_rest(char *temp, int start)
{
	char	*rest;
	int		len;

	len = ft_strlen(temp) - start;
	if (len <= 0)
	{
		free(temp);
		return (NULL);
	}
	rest = ft_substr(temp, start, len);
	free(temp);
	return (rest);
}

static char	*extract_line(char *temp, int *next_start)
{
	char	*line;
	int		i;

	i = 0;
	while (temp[i] && temp[i] != '\n')
		i++;
	if (temp[i] == '\n')
		*next_start = i + 1;
	else
	{
		*next_start = i;
		if (i == 0 && temp[i] == '\0')
			return (NULL);
	}
	line = ft_substr(temp, 0, *next_start);
	return (line);
}

static char	*new_line(int fd, char *buf, char *temp)
{
	int		read_bytes;
	char	*new_temp;

	while (!ft_strchr(temp, '\n'))
	{
		read_bytes = read(fd, buf, BUFFER_SIZE);
		if (read_bytes == 0 && temp[0] == '\0')
		{
			free(temp);
			return (NULL);
		}
		if (read_bytes <= 0)
			return (temp);
		buf[read_bytes] = '\0';
		new_temp = ft_strjoin(temp, buf);
		free(temp);
		temp = new_temp;
	}
	return (temp);
}

char	*get_next_line(int fd)
{
	char			*buf;
	char			*line;
	static char		*temp;
	int				next_start;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (temp == NULL)
		temp = ft_strdup("");
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	temp = new_line(fd, buf, temp);
	free(buf);
	if (!temp)
		return (NULL);
	line = extract_line(temp, &next_start);
	if (!line && !temp)
		return (NULL);
	temp = keep_rest(temp, next_start);
	return (line);
}
