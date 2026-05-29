/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 11:55:22 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/24 17:17:59 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	map_height(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		i++;
	return (i);
}

static void	check_horizontal_walls(char **map, int width, int height)
{
	int	i;

	i = 0;
	while (i < width)
	{
		if (map[0][i] != '1' || map[height - 1][i] != '1')
			error_message("Map error: not closed by walls at top/bottom.");
		i++;
	}
}

static void	check_vertical_walls(char **map, int width, int height)
{
	int	y;

	y = 1;
	while (y < height - 1)
	{
		if (map[y][0] != '1' || map[y][width - 1] != '1')
			error_message("Map error: not closed by walls on the sides.");
		y++;
	}
}

static void	check_map_symbols(char **map, int *player_count,
			int *exit_count, int *collectible_count)
{
	int		x;
	int		y;

	x = 0;
	while (map[x])
	{
		y = 0;
		while (map[x][y])
		{
			if (map[x][y] == 'P')
				(*player_count)++;
			else if (map[x][y] == 'E')
				(*exit_count)++;
			else if (map[x][y] == 'C')
				(*collectible_count)++;
			else if (map[x][y] != '0' && map[x][y] != '1')
				error_message("Map error: invalid character in map!");
			y++;
		}
		x++;
	}
}

void	validate_map(char **map)
{
	int	width;
	int	height;
	int	player_count;
	int	exit_count;
	int	collectible_count;

	width = ft_strlen(map[0]);
	height = map_height(map);
	player_count = 0;
	exit_count = 0;
	collectible_count = 0;
	check_horizontal_walls(map, width, height);
	check_vertical_walls(map, width, height);
	check_map_symbols(map, &player_count, &exit_count, &collectible_count);
	if (player_count != 1)
		error_message("Map error: must be only one player (P) in the map.");
	if (exit_count != 1)
		error_message("Map error: must be only one exit (E) in the map.");
	if (collectible_count < 1)
		error_message("Map error: missing at least one collectible (C).");
}
