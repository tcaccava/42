/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:49:02 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/05/19 19:49:04 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

static char	find_direction(t_player *player)
{
	if (player->dir_x == 0 && player->dir_y == -1)
		player->facing_dir = 'N';
	else if (player->dir_x == 1 && player->dir_y == 0)
		player->facing_dir = 'E';
	else if (player->dir_x == 0 && player->dir_y == 1)
		player->facing_dir = 'S';
	else if (player->dir_x == -1 && player->dir_y == 0)
		player->facing_dir = 'W';
	else if (player->dir_x <= 0 && player->dir_y <= 0)
		player->facing_dir = 'W';
	else if (player->dir_x >= 0 && player->dir_y <= 0)
		player->facing_dir = 'N';
	else if (player->dir_x >= 0 && player->dir_y >= 0)
		player->facing_dir = 'E';
	else if (player->dir_x <= 0 && player->dir_y >= 0)
		player->facing_dir = 'S';
	return (player->facing_dir);
}

static void	define_move(int kcode, double *temp_x, double *temp_y, t_data *data)
{
	double	move_x;
	double	move_y;

	move_x = (data->player->dir_x * 0.1);
	move_y = (data->player->dir_y * 0.1);
	if (kcode == 'w')
	{
		*temp_x = data->player->pos_x + move_x;
		*temp_y = data->player->pos_y + move_y;
	}
	if (kcode == 'a')
	{
		*temp_x = data->player->pos_x + move_y;
		*temp_y = data->player->pos_y - move_x;
	}
	if (kcode == 's')
	{
		*temp_x = data->player->pos_x - move_x;
		*temp_y = data->player->pos_y - move_y;
	}
	if (kcode == 'd')
	{
		*temp_x = data->player->pos_x - move_y;
		*temp_y = data->player->pos_y + move_x;
	}
}

static void	move_player(int keycode, t_data *data, t_map *map)
{
	double	temp_x;
	double	temp_y;

	define_move(keycode, &temp_x, &temp_y, data);
	if (map->map[(int)temp_y][(int)temp_x]
		&& map->map[(int)temp_y][(int)temp_x] != '1'
		&& map->map[(int)temp_y][(int)temp_x] != ' '
		&& map->map[(int)temp_y][(int)temp_x] != '\n'
		&& map->map[(int)temp_y][(int)temp_x] != 'P')
	{
		data->player->pos_x = temp_x;
		data->player->pos_y = temp_y;
		draw(data);
	}
}

int	rotate_player(int k, t_data *data, char dir)
{
	dir = find_direction(data->player);
	if ((k == 65361 && dir == 'N') || (k == 65363 && dir == 'S'))
	{
		data->player->dir_x -= 0.1;
		data->player->dir_y -= 0.1;
	}
	else if ((k == 65361 && dir == 'E') || (k == 65363 && dir == 'W'))
	{
		data->player->dir_x += 0.1;
		data->player->dir_y -= 0.1;
	}
	else if ((k == 65361 && dir == 'S') || (k == 65363 && dir == 'N'))
	{
		data->player->dir_x += 0.1;
		data->player->dir_y += 0.1;
	}
	else if ((k == 65361 && dir == 'W') || (k == 65363 && dir == 'E'))
	{
		data->player->dir_x -= 0.1;
		data->player->dir_y += 0.1;
	}
	else
		return (0);
	norm_direction(data);
	return (1);
}

int	key_hook(int keycode, t_data *data)
{
	static char	dir;

	if (keycode == 32)
		see_in_front(data);
	if (keycode == XK_Escape)
		free_exit(data);
	if (rotate_player(keycode, data, dir) == 1)
		draw(data);
	if (keycode == 'w' || keycode == 'd' || keycode == 's' || keycode == 'a')
	{
		move_player(keycode, data, data->map);
		return (0);
	}
	return (0);
}
