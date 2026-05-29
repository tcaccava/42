/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aseveri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 17:03:14 by aseveri           #+#    #+#             */
/*   Updated: 2026/05/03 02:20:45 by aseveri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	free_and_null(char **arg)
{
	free(*arg);
	*arg = NULL;
}

static int	init_gnl(char **buffer, char **str, char **temp)
{
	char	saved;

	*str = NULL;
	if (*buffer)
	{
		temp[0] = ft_strchr(*buffer, '\n');
		if (temp[0][1])
		{
			temp[1] = ft_strchr(temp[0] + 1, '\n');
			if (temp[1])
			{
				temp[0][0] = 'a';
				saved = temp[1][1];
				temp[1][1] = '\0';
			}
			*str = ft_strdup(temp[0] + 1);
			if (!*str)
				return (free_and_null(buffer), -1);
			if (temp[1])
				return (temp[1][1] = saved, 1);
		}
		return (0);
	}
	*buffer = (char *) malloc((BUFFER_SIZE + 1) * sizeof(char));
	return (!!*buffer - 1);
}

static int	read_no_str(char **buffer, char **str, char **temp)
{
	temp[0] = ft_strchr(*buffer, '\n');
	if (!temp[0])
	{
		*str = ft_strdup(*buffer);
		if (!*str)
			return (free_and_null(buffer), -1);
		return (0);
	}
	*str = malloc((temp[0] - *buffer + 2) * sizeof(char));
	if (!*str)
		return (free_and_null(buffer), -1);
	ft_strlcpy(*str, *buffer, temp[0] - *buffer + 2);
	return (1);
}

static int	read_with_str(char **buffer, char **str, char **temp)
{
	char	saved;

	temp[0] = ft_strchr(*buffer, '\n');
	if (!temp[0])
	{
		temp[1] = ft_strjoin(*str, *buffer);
		free_and_null(str);
		if (!temp[1])
			return (free_and_null(buffer), -1);
		*str = temp[1];
		return (0);
	}
	saved = temp[0][1];
	temp[0][1] = '\0';
	temp[1] = ft_strjoin(*str, *buffer);
	temp[0][1] = saved;
	free(*str);
	*str = temp[1];
	if (!*str)
		return (free_and_null(buffer), -1);
	return (1);
}

char	*get_next_line(int fd)
{
	static char	*buffer[OPEN_MAX];
	char		*str;
	char		*temp[2];
	int			n_byte;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (init_gnl(&buffer[fd], &str, temp))
		return (str);
	while (1)
	{
		n_byte = read(fd, buffer[fd], BUFFER_SIZE);
		if (n_byte < 0)
			return (free_and_null(&buffer[fd]), free_and_null(&str), NULL);
		if (!n_byte)
			return (free_and_null(&buffer[fd]), str);
		buffer[fd][n_byte] = '\0';
		if (!str)
		{
			if (read_no_str(&buffer[fd], &str, temp))
				return (str);
		}
		else if (read_with_str(&buffer[fd], &str, temp))
			return (str);
	}
}
