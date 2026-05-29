/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_b.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 17:06:23 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 09:53:09 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	free_player_animations(t_game *game)
{
	int	i;

	i = 0;
	while (i < IDLE_FRAMES)
	{
		if (game->images.player_anim.idle_frames[i])
			mlx_destroy_image(game->mlx,
				game->images.player_anim.idle_frames[i]);
		i++;
	}
}

void	free_enemy_animations(t_game *game)
{
	int	i;

	i = 0;
	while (i < IDLE_FRAMES)
	{
		if (game->images.enemy_anim.n_idle_frames[i])
			mlx_destroy_image(game->mlx,
				game->images.enemy_anim.n_idle_frames[i]);
		i++;
	}
}

void	free_images(t_game *game)
{
	if (game->images.wall)
		mlx_destroy_image(game->mlx, game->images.wall);
	if (game->images.floor)
		mlx_destroy_image(game->mlx, game->images.floor);
	if (game->images.collectible)
		mlx_destroy_image(game->mlx, game->images.collectible);
	if (game->images.exit)
		mlx_destroy_image(game->mlx, game->images.exit);
	if (game->images.game_over && game->mlx)
		mlx_destroy_image(game->mlx, game->images.game_over);
	free_player_animations(game);
	free_enemy_animations(game);
}

void	free_window(t_game *game)
{
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
}

void	free_map(t_game *game)
{
	int	i;

	if (!game->map)
		return ;
	i = 0;
	while (game->map[i])
		free(game->map[i++]);
	free(game->map);
}
