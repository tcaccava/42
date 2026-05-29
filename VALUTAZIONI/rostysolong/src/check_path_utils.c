/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:44:03 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:44:04 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Checks if position is valid for flood fill */
static int	is_valid_pos(t_flood_data *data, int x, int y)
{
	if (x < 0 || y < 0 || x >= data->game->map.width
		|| y >= data->game->map.height || data->map[y][x] == '1'
		|| data->map[y][x] == 'V')
		return (0);
	return (1);
}

/* Recursive flood fill to find all collectibles */
void	flood_fill_collect(t_flood_data *data, int x, int y)
{
	if (!is_valid_pos(data, x, y))
		return ;
	if (data->map[y][x] == 'C')
		data->collectibles++;
	data->map[y][x] = 'V';
	flood_fill_collect(data, x + 1, y);
	flood_fill_collect(data, x - 1, y);
	flood_fill_collect(data, x, y + 1);
	flood_fill_collect(data, x, y - 1);
}

/* Recursive flood fill to find the exit */
void	flood_fill_exit(t_flood_data *data, int x, int y)
{
	if (!is_valid_pos(data, x, y))
		return ;
	if (data->map[y][x] == 'E')
	{
		data->exit_found = 1;
		return ;
	}
	data->map[y][x] = 'V';
	flood_fill_exit(data, x + 1, y);
	flood_fill_exit(data, x - 1, y);
	flood_fill_exit(data, x, y + 1);
	flood_fill_exit(data, x, y - 1);
}
