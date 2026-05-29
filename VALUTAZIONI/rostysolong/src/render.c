/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:51:00 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:51:02 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Places a single tile image at the specified coordinates */
void	render_tile(t_game *game, t_img img, int x, int y)
{
	mlx_put_image_to_window(game->mlx, game->win, img.img,
		x * TILE_SIZE, y * TILE_SIZE);
}

/* Renders a ground tile and any objects on it */
static void	render_ground_objects(t_game *game, int i, int j,
	int player_on_exit)
{
	render_tile(game, game->floor, j, i);
	if (game->map.grid[i][j] == 'C')
		render_tile(game, game->collect, j, i);
	else if (game->map.grid[i][j] == 'E')
	{
		render_tile(game, game->exit, j, i);
		if (player_on_exit && j == game->map.exit_pos.x
			&& i == game->map.exit_pos.y)
			render_tile(game, game->player, j, i);
	}
	else if (game->map.grid[i][j] == 'P' && !player_on_exit)
		render_tile(game, game->player, j, i);
}

/* Renders the main game map with all elements */
void	render_game(t_game *game)
{
	int	i;
	int	j;
	int	player_on_exit;

	player_on_exit = (game->map.player_pos.x == game->map.exit_pos.x
			&& game->map.player_pos.y == game->map.exit_pos.y);
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == '1')
				render_tile(game, game->wall, j, i);
			else
				render_ground_objects(game, i, j, player_on_exit);
			j++;
		}
		i++;
	}
	update_window(game);
}
