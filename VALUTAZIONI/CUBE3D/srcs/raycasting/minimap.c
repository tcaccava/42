/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:46:35 by ldei-sva          #+#    #+#             */
/*   Updated: 2025/05/19 19:46:39 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

void	define_values(t_minimap *minimap, t_data *data)
{
	minimap->start_x = data->player->pos_x - 10;
	if (minimap->start_x < 0)
		minimap->start_x = 0;
	minimap->start_y = data->player->pos_y - data->map->map_start - 10;
	if (minimap->start_y < 0)
		minimap->start_y = 0;
	minimap->end_x = data->player->pos_x + 10;
	if (minimap->end_x > data->map->map_width)
		minimap->end_x = data->map->map_width;
	minimap->end_y = data->player->pos_y - data->map->map_start + 10;
	if (minimap->end_y > data->map->map_height)
		minimap->end_y = data->map->map_height;
}

void	fill_square(int x, int y, t_data *data, int color)
{
	int		len;
	int		height;
	int		x1;
	int		y1;

	height = 0;
	while (height < 10)
	{
		len = 0;
		while (len < 10)
		{
			x1 = WIDTH - 210 + x + len;
			y1 = HEIGHT - 210 + y + height;
			if (x1 < WIDTH && x1 > 0 && y1 < HEIGHT && y1 > 0)
				my_mlx_pixel_put(data, x1, y1, color);
			len++;
		}
		height++;
	}
}

void	draw_mini_map(t_data *data)
{
	char				**map;
	static t_minimap	mini;
	int					x;
	int					y;

	map = data->map->map + data->map->map_start;
	define_values(&mini, data);
	y = mini.start_y;
	while (map[y] && y < mini.end_y && y < HEIGHT)
	{
		x = mini.start_x;
		while ((int)ft_strlen(map[y]) > x && x < mini.end_x && x < WIDTH)
		{
			if (map[y][x] == '1')
				fill_square((x - mini.start_x) * 10, \
				(y - mini.start_y) * 10, data, 0xFFFFFF);
			else if (map[y][x] != ' ' && map[y][x] != '\n')
				fill_square((x - mini.start_x) * 10, \
				(y - mini.start_y) * 10, data, 0x000000);
			x++;
		}
		y++;
	}
	fill_square((data->player->pos_x - mini.start_x) * 10, (data->player->pos_y \
	- data->map->map_start - mini.start_y) * 10, data, 0xFFFF00);
}
