/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:00:43 by fionni            #+#    #+#             */
/*   Updated: 2026/02/05 21:00:43 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	sl_validate_extension(char *file)
{
	int	len;

	len = ft_strlen(file);
	if (len < 4)
		return (0);
	if (file[len - 1] != 'r')
		return (0);
	if (file[len - 2] != 'e')
		return (0);
	if (file[len - 3] != 'b')
		return (0);
	if (file[len - 4] != '.')
		return (0);
	return (1);
}

int	sl_open_map(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		sl_perror("Cannot open map");
	return (fd);
}

char	*sl_read_line(int fd)
{
	char	buf[2];
	char	*line;
	char	*tmp;
	int		n;

	line = ft_strdup("");
	if (!line)
		return (NULL);
	buf[1] = '\0';
	n = read(fd, buf, 1);
	while (n > 0)
	{
		tmp = ft_strjoin(line, buf);
		free(line);
		line = tmp;
		if (buf[0] == '\n')
			break ;
		n = read(fd, buf, 1);
	}
	if (n <= 0 && line[0] == '\0')
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*sl_read_file(int fd)
{
	char	*all;
	char	*line;
	char	*tmp;

	all = ft_strdup("");
	if (!all)
		return (NULL);
	line = sl_read_line(fd);
	while (line)
	{
		tmp = ft_strjoin(all, line);
		free(all);
		free(line);
		all = tmp;
		line = sl_read_line(fd);
	}
	return (all);
}

char	*sl_load_raw_map(char *file)
{
	int		fd;
	char	*raw;

	if (!sl_validate_extension(file))
		sl_error("Invalid file extension (must be .ber)");
	fd = sl_open_map(file);
	raw = sl_read_file(fd);
	close(fd);
	if (!raw || raw[0] == '\0')
		sl_error("Map file is empty");
	return (raw);
}
