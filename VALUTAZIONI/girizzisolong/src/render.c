/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:49:12 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/26 23:41:12 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	render_tile(t_game *game, int x, int y, char tile)
{
	void	*image;

	mlx_put_image_to_window(game->mlx, game->win, game->images.floor,
		x * TILE_SIZE, y * TILE_SIZE);
	if (tile == '1')
		image = game->images.wall;
	else if (tile == 'P')
		image = game->images.player;
	else if (tile == 'C')
		image = game->images.collectible;
	else if (tile == 'E')
		image = game->images.exit;
	else
		return ;
	mlx_put_image_to_window(game->mlx, game->win, image,
		x * TILE_SIZE, y * TILE_SIZE);
}

void	render_map(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			render_tile(game, x, y, game->map[y][x]);
			x++;
		}
		y++;
	}
}
