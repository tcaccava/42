/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_play.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:20:11 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/24 20:52:42 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static char	**copy_map(char **map)
{
	char	**copy;
	int		height;
	int		i;

	height = 0;
	while (map[height])
		height++;
	copy = malloc(sizeof(char *) * (height + 1));
	if (!copy)
		error_message("Error allocating memory for map copy!");
	i = 0;
	while (i < height)
	{
		copy[i] = ft_strdup(map[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			error_message("Error copying map!");
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	free_map_copy(char **map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (map[i])
		free(map[i++]);
	free(map);
}

static void	find_player(char **map, int *px, int *py)
{
	int	x;
	int	y;

	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'P')
			{
				*px = x;
				*py = y;
				return ;
			}
			x++;
		}
		y++;
	}
}

static void	flood_fill_recursive(char **map, int x, int y, t_flood_data *data)
{
	if (x < 0 || y < 0 || !map[y] || !map[y][x])
		return ;
	if (map[y][x] == '1' || map[y][x] == 'V')
		return ;
	if (map[y][x] == 'C')
		data->collectibles++;
	if (map[y][x] == 'E')
		data->exits++;
	map[y][x] = 'V';
	flood_fill_recursive(map, x + 1, y, data);
	flood_fill_recursive(map, x - 1, y, data);
	flood_fill_recursive(map, x, y + 1, data);
	flood_fill_recursive(map, x, y - 1, data);
}

void	validate_playability(char **map)
{
	char			**map_copy;
	int				player_x;
	int				player_y;
	t_flood_data	data;

	map_copy = copy_map(map);
	find_player(map, &player_x, &player_y);
	data.collectibles = 0;
	data.exits = 0;
	flood_fill_recursive(map_copy, player_x, player_y, &data);
	if (data.collectibles != count_collectibles(map))
	{
		free_map_copy(map_copy);
		error_message("Map error: not all collectibles are reachable!");
	}
	if (data.exits < 1)
	{
		free_map_copy(map_copy);
		error_message("Map error: no exit is reachable!");
	}
	free_map_copy(map_copy);
}
