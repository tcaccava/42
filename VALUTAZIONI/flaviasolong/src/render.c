/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 16:25:51 by faiello           #+#    #+#             */
/*   Updated: 2025/10/22 01:59:21 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdio.h>

void	_tile(t_game *game, t_texture *texture, int x, int y)
{
	if (!texture)
	{
		printf("Error: Texture is NULL\n");
		return ;
	}
	mlx_put_image_to_window(game->mlx, game->win, texture->img, x * TILE_SIZE, y
		* TILE_SIZE);
}

void	render_game(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (game->map.map[y][x] == '1')
				_tile(game, &game->assets.wall, x, y);
			else if (game->map.map[y][x] == '0')
				_tile(game, &game->assets.floor, x, y);
			else if (game->map.map[y][x] == 'C')
				_tile(game, &game->assets.collectible, x, y);
			else if (game->map.map[y][x] == 'E' && game->exit_is_open)
				_tile(game, &game->assets.exit_closed, x, y);
			else if (game->map.map[y][x] == 'E' && !game->exit_is_open)
				_tile(game, &game->assets.exit_closed, x, y);
			x++;
		}
		y++;
	}
	_tile(game, &game->assets.player, game->map.player.x, game->map.player.y);
}
