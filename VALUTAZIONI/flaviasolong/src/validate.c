/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 16:29:26 by faiello           #+#    #+#             */
/*   Updated: 2025/10/17 16:31:10 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "so_long.h"
#include <stdbool.h>

static bool	check_walls(t_map *map, int x, int y, char tile)
{
	if (y == 0 || y == map->height - 1 || x == 0 || x == map->width - 1)
		return (tile == '1');
	return (true);
}

static bool	process_row(t_map *map, int y, int *player_count,
				int *exit_count)
{
	int		x;
	char	tile;

	x = 0;
	while (x < map->width)
	{
		tile = map->map[y][x];
		if (!check_walls(map, x, y, tile))
			return (false);
		if (tile == 'P')
		{
			*player_count = *player_count + 1;
			map->player.x = x;
			map->player.y = y;
			map->map[y][x] = '0';
		}
		else if (tile == 'E')
			*exit_count = *exit_count + 1;
		else if (tile == 'C')
			map->collectibles = map->collectibles + 1;
		else if (tile != '0' && tile != '1')
			return (false);
		x++;
	}
	return (true);
}

bool	validate_map(t_map *map)
{
	int	player_count;
	int	exit_count;
	int	y;

	if (!map || !map->map || map->width < 3 || map->height < 3)
		return (false);
	player_count = 0;
	exit_count = 0;
	map->collectibles = 0;
	y = 0;
	while (y < map->height)
	{
		if ((int)ft_strlen(map->map[y]) != map->width)
			return (false);
		if (!process_row(map, y, &player_count, &exit_count))
			return (false);
		y++;
	}
	if (player_count != 1 || exit_count != 1 || map->collectibles == 0)
		return (false);
	return (true);
}
