/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:42:30 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/23 20:37:22 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	find_player_position(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] == 'P')
			{
				game->player_x = x;
				game->player_y = y;
				return ;
			}
			x++;
		}
		y++;
	}
}

int	count_collectibles(char **map)
{
	int	x;
	int	y;
	int	count;

	count = 0;
	y = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (map[y][x] == 'C')
				count++;
			x++;
		}
		y++;
	}
	return (count);
}

static void	load_images(t_game *game)
{
	int	width;
	int	height;

	game->images.wall = mlx_xpm_file_to_image(game->mlx,
			"sprites/wall.xpm", &width, &height);
	game->images.floor = mlx_xpm_file_to_image(game->mlx,
			"sprites/floor.xpm", &width, &height);
	game->images.player = mlx_xpm_file_to_image(game->mlx,
			"sprites/player.xpm", &width, &height);
	game->images.collectible = mlx_xpm_file_to_image(game->mlx,
			"sprites/collectible.xpm", &width, &height);
	game->images.exit = mlx_xpm_file_to_image(game->mlx,
			"sprites/exit.xpm", &width, &height);
	if (!game->images.wall || !game->images.floor || !game->images.player
		|| !game->images.collectible || !game->images.exit)
		error_message("Error loading image files!");
}

void	init_game_struct(t_game *game, char **map)
{
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		free(game->mlx);
	}
	game->map = map;
	game->map_width = ft_strlen(map[0]);
	game->map_height = 0;
	while (map[game->map_height])
		game->map_height++;
	game->win = mlx_new_window(game->mlx,
			game->map_width * TILE_SIZE,
			game->map_height * TILE_SIZE,
			"So_long");
	if (!game->win)
	{
		free(game->win);
		error_message("Error creating window!");
	}
	load_images(game);
	find_player_position(game);
	game->collectibles = count_collectibles(map);
	game->moves = 0;
}
