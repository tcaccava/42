/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:00:59 by fionni            #+#    #+#             */
/*   Updated: 2026/02/05 21:00:59 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	sl_put_tile(t_game *g, char c, int x, int y)
{
	void	*img;

	img = g->img.floor;
	if (c == '1')
		img = g->img.wall;
	else if (c == 'P')
		img = g->img.player;
	else if (c == 'C')
		img = g->img.collectible;
	else if (c == 'E')
		img = g->img.exit;
	mlx_put_image_to_window(g->mlx, g->win, img, x * 64, y * 64);
}

void	sl_render_row(t_game *g, int y)
{
	int	x;

	x = 0;
	while (x < g->map.width)
	{
		sl_put_tile(g, g->map.grid[y][x], x, y);
		x++;
	}
}

void	sl_render_background(t_game *g)
{
	int	x;
	int	y;

	y = 0;
	while (y < g->map.height)
	{
		x = 0;
		while (x < g->map.width)
		{
			mlx_put_image_to_window(g->mlx, g->win,
				g->img.floor, x * 64, y * 64);
			x++;
		}
		y++;
	}
}

void	sl_render_map(t_game *g)
{
	int	y;

	sl_render_background(g);
	y = 0;
	while (y < g->map.height)
	{
		sl_render_row(g, y);
		y++;
	}
}

void	sl_refresh_window(t_game *g)
{
	mlx_clear_window(g->mlx, g->win);
	sl_render_map(g);
}
