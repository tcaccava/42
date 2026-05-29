/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:06:23 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/25 15:47:55 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	free_images(t_game *game)
{
	if (game->images.wall)
		mlx_destroy_image(game->mlx, game->images.wall);
	if (game->images.floor)
		mlx_destroy_image(game->mlx, game->images.floor);
	if (game->images.player)
		mlx_destroy_image(game->mlx, game->images.player);
	if (game->images.collectible)
		mlx_destroy_image(game->mlx, game->images.collectible);
	if (game->images.exit)
		mlx_destroy_image(game->mlx, game->images.exit);
}

static void	free_window(t_game *game)
{
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}

static void	free_map(t_game *game)
{
	int	i;

	if (!game->map)
		return ;
	i = 0;
	while (game->map[i])
		free(game->map[i++]);
	free(game->map);
}

int	close_game(t_game *game)
{
	free_images(game);
	free_window(game);
	free_map(game);
	exit(EXIT_SUCCESS);
	return (0);
}

void	start_game(t_game *game)
{
	render_map(game);
	mlx_hook(game->win, 17, 0, close_game, game);
	mlx_key_hook(game->win, key_handler, game);
	mlx_loop(game->mlx);
}
