/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dfelicia <dfelicia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 18:54:46 by dfelicia          #+#    #+#             */
/*   Updated: 2026/07/31 18:35:03 by dfelicia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*pointer(char *leftover)
{
	int		i;
	int		n;
	char	*newleftover;

	i = 0;
	n = 0;
	if (!leftover)
		return (NULL);
	while (leftover[i] != '\n')
		i++;
	n = ft_strlen(leftover + ++i);
	if (n == 0)
		return (NULL);
	newleftover = malloc(n + 1);
	if (!newleftover)
		return (NULL);
	n = 0;
	while (leftover[i + n] != '\0')
	{
		newleftover[n] = leftover[i + n];
		n++;
	}
	newleftover[n] = '\0';
	return (newleftover);
}

char	*leftovermkr(char *leftover, char *buffer, int fd)
{
	int	bytes;

	if (ft_strchr(leftover, '\n') == 1)
		return (leftover);
	bytes = read(fd, buffer, BUFFER_SIZE);
	if (bytes <= 0)
		return (leftover);
	buffer[bytes] = '\0';
	leftover = ft_strjoin(leftover, buffer);
	while (ft_strchr(leftover, '\n') == 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes <= 0)
			break ;
		buffer[bytes] = '\0';
		leftover = ft_strjoin(leftover, buffer);
	}
	if (!leftover)
		return (NULL);
	else
		return (leftover);
}

char	*newline(char *leftover)
{
	int		i;
	char	*line;

	i = 0;
	while (leftover[i] != '\n')
		i++;
	line = malloc(i + 2);
	line[++i] = '\0';
	while (--i >= 0)
		line[i] = leftover[i];
	return (line);
}

char	*freebuffer(char *buffer, char *line)
{
	free(buffer);
	if (!line)
		return (NULL);
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*leftover;
	char		*buffer;
	char		*line;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	leftover = leftovermkr(leftover, buffer, fd);
	if (!buffer)
		return (NULL);
	if (ft_strchr(leftover, '\n') == 0)
	{
		line = leftover;
		leftover = NULL;
		return (freebuffer(buffer, line));
	}
	line = newline(leftover);
	temp = leftover;
	leftover = pointer(leftover);
	free (temp);
	return (freebuffer(buffer, line));
}
