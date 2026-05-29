/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_surroundings.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:19:59 by lonulli           #+#    #+#             */
/*   Updated: 2025/05/16 10:20:01 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static void	check_player_surroundings(t_data *data, char **map, int x, int y);
static int	check_horizontal_vertical(t_data *data, char **map, int x, int y);
static int	check_diagonal(t_data *data, char **map, int x, int y);

int	check_zero_surroundings(t_data *data, char **map, int x, int y)
{
	if (!is_coordinate_valid(map, x, y))
		return (0);
	if (check_horizontal_vertical(data, map, x, y) && check_diagonal(data, map,
			x, y))
		return (1);
	return (0);
}

static int	check_horizontal_vertical(t_data *data, char **map, int x, int y)
{
	char	c_up;
	char	c_down;
	char	c_left;
	char	c_right;

	c_up = '\0';
	c_down = '\0';
	c_left = '\0';
	c_right = '\0';
	if (is_coordinate_valid(map, x, y - 1))
		c_up = map[y - 1][x];
	if (is_coordinate_valid(map, x, y + 1))
		c_down = map[y + 1][x];
	if (is_coordinate_valid(map, x - 1, y))
		c_left = map[y][x - 1];
	if (is_coordinate_valid(map, x + 1, y))
		c_right = map[y][x + 1];
	if (are_sorroundings_valid(data, c_up) && are_sorroundings_valid(data,
			c_down) && are_sorroundings_valid(data, c_left)
		&& are_sorroundings_valid(data, c_right))
		return (1);
	return (0);
}

static int	check_diagonal(t_data *data, char **map, int x, int y)
{
	char	c_top_left;
	char	c_top_right;
	char	c_bottom_left;
	char	c_bottom_right;

	c_top_left = '\0';
	c_top_right = '\0';
	c_bottom_left = '\0';
	c_bottom_right = '\0';
	if (is_coordinate_valid(map, x - 1, y - 1))
		c_top_left = map[y - 1][x - 1];
	if (is_coordinate_valid(map, x + 1, y - 1))
		c_top_right = map[y - 1][x + 1];
	if (is_coordinate_valid(map, x - 1, y + 1))
		c_bottom_left = map[y + 1][x - 1];
	if (is_coordinate_valid(map, x + 1, y + 1))
		c_bottom_right = map[y + 1][x + 1];
	if (are_sorroundings_valid(data, c_top_left) && are_sorroundings_valid(data,
			c_top_right) && are_sorroundings_valid(data, c_bottom_left)
		&& are_sorroundings_valid(data, c_bottom_right))
		return (1);
	return (0);
}

int	check_and_set(t_data *data, char c, int x, int y)
{
	char	**map;

	map = data->map->map;
	if (c == 'N' || c == 'W' || c == 'S' || c == 'E')
	{
		data->player->facing_dir = c;
		data->player->pos_x = x + 0.5;
		data->player->pos_y = y + 0.5;
		data->player->exists += 1;
		check_player_surroundings(data, map, x, y);
		return (1);
	}
	else if (c == '0' || c == '1' || c == '\n' || c == '\t' || c == 'P'
		|| c == ' ' || c == 'D')
		return (1);
	return (0);
}

static void	check_player_surroundings(t_data *data, char **map, int x, int y)
{
	int	map_height;

	map_height = data->map->map_height;
	if ((y <= 0 || y - data->map->map_start >= (map_height - 1) \
	|| check_identifier(map[y][x + 1], "01PD") \
	|| check_identifier(map[y][x - 1], "01PD") || check_identifier \
	(map[y - 1][x], "01PD") || check_identifier(map[y + 1][x], "01PD") \
	|| check_identifier(map[y - 1][x + 1], "01PD") \
	|| check_identifier(map[y - 1][x - 1], "01PD") \
	|| check_identifier(map[y + 1][x - 1], "01PD") \
	|| check_identifier(map[y + 1][x + 1], "01PD")))
	{
		data->err_type = E_NO_VAL_SURR;
		print_err_and_free(data, NULL);
	}
}
