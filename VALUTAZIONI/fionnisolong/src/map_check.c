/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:00:18 by fionni            #+#    #+#             */
/*   Updated: 2026/02/05 21:00:18 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	sl_check_rectangle(t_map *map)
{
	int	i;
	int	len;

	i = 0;
	len = map->width;
	while (i < map->height)
	{
		if ((int)ft_strlen(map->grid[i]) != len)
			sl_error("Map is not rectangular");
		i++;
	}
}

void	sl_check_walls(t_map *map)
{
	int	x;
	int	y;

	x = 0;
	while (x < map->width)
	{
		if (map->grid[0][x] != '1'
			|| map->grid[map->height - 1][x] != '1')
			sl_error("Map not closed by walls");
		x++;
	}
	y = 0;
	while (y < map->height)
	{
		if (map->grid[y][0] != '1'
			|| map->grid[y][map->width - 1] != '1')
			sl_error("Map not closed by walls");
		y++;
	}
}

void	sl_update_counts(t_map *map, char c)
{
	if (c == 'P')
		map->players++;
	else if (c == 'E')
		map->exits++;
	else if (c == 'C')
		map->collectibles++;
	else if (c != '1' && c != '0')
		sl_error("Invalid character in map");
}

void	sl_check_contents(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			sl_update_counts(map, map->grid[y][x]);
			x++;
		}
		y++;
	}
	if (map->players != 1)
		sl_error("Map must contain exactly 1 player");
	if (map->exits < 1)
		sl_error("Map must contain at least 1 exit");
	if (map->collectibles < 1)
		sl_error("Map must contain at least 1 collectible");
}

void	sl_check_map(t_map *map)
{
	sl_check_rectangle(map);
	sl_check_walls(map);
	sl_check_contents(map);
}
