/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:42:30 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 09:49:49 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

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

void	init_enemies(t_game *game)
{
	game->enemy_count = count_enemies(game->map);
	if (game->enemy_count == 0)
	{
		game->enemies = NULL;
		return ;
	}
	game->enemies = malloc(sizeof(t_enemy) * game->enemy_count);
	if (!game->enemies)
		error_message("Error allocating memory for enemies!");
	find_all_enemies(game);
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
	game->images.collectible = mlx_xpm_file_to_image(game->mlx,
			"sprites/collectible.xpm", &width, &height);
	game->images.exit = mlx_xpm_file_to_image(game->mlx,
			"sprites/exit.xpm", &width, &height);
	game->images.game_over = NULL;
	init_player_animations(game);
	init_enemy_animations(game);
	if (!game->images.wall || !game->images.floor
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
	init_enemies(game);
	game->collectibles = count_collectibles(map);
	game->moves = 0;
	game->is_game_over = 0;
}
