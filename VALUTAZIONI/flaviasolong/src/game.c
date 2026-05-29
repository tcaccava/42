/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:56:49 by faiello           #+#    #+#             */
/*   Updated: 2025/10/17 16:04:44 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

#define PATH_WALL "assets/wall.xpm"
#define PATH_FLOOR "assets/floor.xpm"
#define PATH_PLAYER "assets/player.xpm"
#define PATH_COLLECTIBLE "assets/collectible.xpm"
#define PATH_EXIT_CLOSED "assets/exit_closed.xpm"
#define PATH_EXIT_OPEN "assets/exit_open.xpm"

static bool	load_texture(t_game *game, t_texture *texture, char const *path)
{
	texture->img = mlx_xpm_file_to_image(game->mlx, (char *)path,
			&texture->width, &texture->height);
	if (!texture->img)
		return (false);
	return (true);
}

static bool	load_assets(t_game *game)
{
	if (!load_texture(game, &game->assets.wall, PATH_WALL))
		return (false);
	if (!load_texture(game, &game->assets.floor, PATH_FLOOR))
		return (false);
	if (!load_texture(game, &game->assets.player, PATH_PLAYER))
		return (false);
	if (!load_texture(game, &game->assets.collectible, PATH_COLLECTIBLE))
		return (false);
	if (!load_texture(game, &game->assets.exit_closed, PATH_EXIT_CLOSED))
		return (false);
	if (!load_texture(game, &game->assets.exit_open, PATH_EXIT_OPEN))
		return (false);
	return (true);
}

static void	destroy_texture(t_game *game, t_texture *texture)
{
	if (texture->img)
	{
		mlx_destroy_image(game->mlx, texture->img);
		texture->img = NULL;
	}
}

bool	init_game(t_game *game, const char *title)
{
	int	width;
	int	height;

	width = game->map.width * TILE_SIZE;
	height = game->map.height * TILE_SIZE;
	game->mlx = mlx_init();
	if (!game->mlx)
		return (false);
	game->win = mlx_new_window(game->mlx, width, height, (char *)title);
	if (!game->win)
	{
		destroy_game(game);
		return (false);
	}
	if (!load_assets(game))
	{
		destroy_game(game);
		return (false);
	}
	game->moves = 0;
	game->collected = 0;
	game->exit_is_open = false;
	return (true);
}

void	destroy_game(t_game *game)
{
	if (!game)
		return ;
	if (game->mlx)
	{
		destroy_texture(game, &game->assets.player);
		destroy_texture(game, &game->assets.collectible);
		destroy_texture(game, &game->assets.exit_closed);
		destroy_texture(game, &game->assets.exit_open);
		destroy_texture(game, &game->assets.floor);
		destroy_texture(game, &game->assets.wall);
		if (game->win)
		{
			mlx_destroy_window(game->mlx, game->win);
			game->win = NULL;
		}
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
	free_map(&game->map);
}
