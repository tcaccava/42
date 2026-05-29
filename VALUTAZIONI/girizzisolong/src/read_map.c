/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:17:55 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/26 23:39:29 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	count_lines(char *file)
{
	int		fd;
	int		count;
	char	buffer[1];
	int		bytes_read;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		error_message("Error 1 opening map file!");
	count = 0;
	bytes_read = read(fd, buffer, 1);
	while (bytes_read > 0)
	{
		if (buffer[0] == '\n')
			count++;
		bytes_read = read(fd, buffer, 1);
	}
	lseek(fd, -1, SEEK_END);
	bytes_read = read(fd, buffer, 1);
	if (buffer[0] != '\n')
		count++;
	close(fd);
	return (count);
}

static void	remove_newline(char *str)
{
	int	len;

	if (!str)
		return ;
	len = ft_strlen(str);
	if (len > 0 && str[len - 1] == '\n')
		str[len - 1] = '\0';
}

static char	**alloc_map(char *file, int *lines)
{
	char	**map;

	*lines = count_lines(file);
	if (*lines == 0)
		error_message("Map file is empty!");
	map = malloc(sizeof(char *) * (*lines + 1));
	if (!map)
	{
		free(map);
		error_message("Error allocating memory for the map!");
	}
	return (map);
}

static void	read_map_lines(int fd, char **map, int lines)
{
	int	i;
	int	row_len;

	i = 0;
	row_len = -1;
	while (i < lines)
	{
		map[i] = read_single_line(fd);
		if (!map[i])
			error_message("Error reading map file!");
		remove_newline(map[i]);
		if (row_len == -1)
			row_len = ft_strlen(map[i]);
		else if ((int)ft_strlen(map[i]) != row_len)
			error_message("Map is not rectangular!");
		i++;
	}
	map[i] = NULL;
}

char	**read_map(char *file)
{
	int		fd;
	int		lines;
	char	**map;

	map = alloc_map(file, &lines);
	fd = open(file, O_RDONLY);
	if (fd < 0)
		error_message("Error 2 opening map file!");
	read_map_lines(fd, map, lines);
	close(fd);
	return (map);
}
