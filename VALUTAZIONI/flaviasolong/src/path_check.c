/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 23:32:04 by faiello           #+#    #+#             */
/*   Updated: 2025/10/17 16:02:28 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdbool.h>
#include <stdlib.h>

static bool	initialize_resources(t_map *map, t_player **queue, int **visited,
		int *max)
{
	*max = map->width * map->height;
	*queue = (t_player *)ft_calloc(*max, sizeof(t_player));
	if (!*queue)
		return (false);
	*visited = (int *)ft_calloc(*max, sizeof(int));
	if (!*visited)
	{
		free(*queue);
		return (false);
	}
	return (true);
}

static void	flood_fill(t_map *map, int *visited, int x, int y)
{
	if (x < 0 || x >= map->width)
		return ;
	if (y < 0 || y >= map->height)
		return ;
	if (map->map[y][x] == '1')
		return ;
	if (visited[y * map->width + x])
		return ;
	visited[y * map->width + x] = 1;
	flood_fill(map, visited, x + 1, y);
	flood_fill(map, visited, x - 1, y);
	flood_fill(map, visited, x, y + 1);
	flood_fill(map, visited, x, y - 1);
}

static void	fill_visited_recursive(t_map *map, int *visited)
{
	int	x;
	int	y;

	x = map->player.x;
	y = map->player.y;
	flood_fill(map, visited, x, y);
}

static bool	scan_unreachable_items(t_map *map, int *visited)
{
	int	y;
	int	x;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if ((map->map[y][x] == 'C' || map->map[y][x] == 'E') && !visited[y
				* map->width + x])
				return (false);
			x++;
		}
		y++;
	}
	return (true);
}

bool	check_map_path(t_map *map)
{
	int			max;
	t_player	*queue;
	int			*visited;
	bool		ok;

	if (!map || !map->map)
		return (false);
	if (!initialize_resources(map, &queue, &visited, &max))
		return (false);
	fill_visited_recursive(map, visited);
	ok = scan_unreachable_items(map, visited);
	free(queue);
	free(visited);
	return (ok);
}
