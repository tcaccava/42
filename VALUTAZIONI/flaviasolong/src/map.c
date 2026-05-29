/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 16:13:14 by faiello           #+#    #+#             */
/*   Updated: 2025/10/17 16:25:16 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <fcntl.h>

static bool	append_row_or_fail(t_map *map, char *line)
{
	char	**tmp;
	int		i;

	tmp = (char **)ft_calloc(map->height + 2, sizeof(char *));
	if (!tmp)
	{
		free(line);
		free_map(map);
		return (false);
	}
	i = 0;
	while (i < map->height)
	{
		tmp[i] = map->map[i];
		i++;
	}
	tmp[map->height] = line;
	free(map->map);
	map->map = tmp;
	map->height++;
	return (true);
}

static bool	process_line(t_map *map, char *line)
{
	size_t	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
	{
		line[len - 1] = '\0';
		len = len - 1;
	}
	if (len == 0)
	{
		free(line);
		return (false);
	}
	if (map->height == 0)
		map->width = (int)len;
	else if ((int)len != map->width)
	{
		free(line);
		return (false);
	}
	if (!append_row_or_fail(map, line))
		return (false);
	return (true);
}

static bool	process_file(int fd, t_map *map)
{
	char	*line;

	line = get_next_line(fd);
	while ((line))
	{
		if (!process_line(map, line))
		{
			free_map(map);
			close(fd);
			return (false);
		}
		line = get_next_line(fd);
	}
	close(fd);
	return (true);
}

bool	load_map(const char *path, t_map *map)
{
	int		fd;

	ft_bzero(map, sizeof(t_map));
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	if (!process_file(fd, map))
		return (false);
	if (map->height == 0 || map->width == 0)
	{
		free_map(map);
		return (false);
	}
	return (true);
}

void	free_map(t_map *map)
{
	int	i;

	if (!map || !map->map)
		return ;
	i = 0;
	while (i < map->height)
	{
		free(map->map[i]);
		i++;
	}
	free(map->map);
	map->map = NULL;
	map->height = 0;
	map->width = 0;
	map->collectibles = 0;
}
