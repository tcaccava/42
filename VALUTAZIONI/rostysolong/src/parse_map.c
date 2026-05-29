/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:47:24 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:47:25 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Checks if the file has a valid .ber extension */
int	check_file_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 5)
		return (0);
	if (filename[0] == '.')
		return (0);
	if (ft_strncmp(&filename[len - 4], ".ber", 4) != 0)
		return (0);
	return (1);
}

/* Parses the map file and initializes the game map data */
int	parse_map(t_game *game, char *map_path)
{
	if (!check_file_extension(map_path))
		return (exit_game(game, "Bad extension", 0));
	if (!read_map(game, map_path))
		return (0);
	return (1);
}

/* Reads line by line from map file and joins into content string */
static char	*read_file_content(int fd)
{
	char	*line;
	char	*map_content;
	char	*temp;

	map_content = ft_strdup("");
	if (!map_content)
		return (NULL);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		temp = map_content;
		map_content = ft_strjoin(map_content, line);
		free(temp);
		free(line);
		if (!map_content)
			return (NULL);
	}
	return (map_content);
}

/* Reads the map content from file and processes it */
int	read_map(t_game *game, char *map_path)
{
	int		fd;
	char	*map_content;

	fd = open(map_path, O_RDONLY);
	if (fd == -1)
		return (exit_game(game, "No exist map", 0));
	map_content = read_file_content(fd);
	close(fd);
	if (!map_content)
		return (exit_game(game, "Memory allocation failed", 0));
	get_map_dimensions(game, map_content);
	game->map.grid = create_map_grid(game, map_content);
	free(map_content);
	return (game->map.grid != NULL);
}
