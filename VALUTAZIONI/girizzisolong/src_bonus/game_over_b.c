/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_over_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 07:42:46 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 09:50:45 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static void	load_game_over_image(t_game *game)
{
	int	width;
	int	height;

	if (game->images.game_over)
		return ;
	game->images.game_over = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/game_over.xpm", &width, &height);
	if (!game->images.game_over)
		error_message("Error loading game over image!");
}

static void	convert_map_to_floor(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			if (x == game->player_x && y == game->player_y)
				game->map[y][x] = 'G';
			else
				game->map[y][x] = '0';
			x++;
		}
		y++;
	}
}

void	game_over(t_game *game)
{
	ft_putendl_fd("\033[0;31mGame Over! Scarabocchio took you!", STDOUT_FILENO);
	load_game_over_image(game);
	convert_map_to_floor(game);
	game->is_game_over = 1;
	game->enemy_count = 0;
	if (game->enemies)
	{
		free(game->enemies);
		game->enemies = NULL;
	}
	render_map(game);
}
