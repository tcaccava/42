/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop_b.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 03:13:33 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 09:48:26 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

int	animation_loop(t_game *game)
{
	static long	last_render_time = 0;
	static int	move_counter = 0;
	long		current_time;

	current_time = get_current_time_microseconds();
	if (move_counter++ >= ENEMY_SPEED)
	{
		move_counter = 0;
		update_enemies_on_map(game);
		move_enemies_patrol(game);
	}
	if (current_time - last_render_time >= 16667)
	{
		render_map(game);
		last_render_time = current_time;
	}
	return (0);
}

void	start_game(t_game *game)
{
	render_map(game);
	mlx_hook(game->win, 17, 0, close_game, game);
	mlx_key_hook(game->win, key_handler, game);
	mlx_loop_hook(game->mlx, animation_loop, game);
	mlx_loop(game->mlx);
}

int	close_game(t_game *game)
{
	free_images(game);
	free_enemies(game);
	free_window(game);
	free_map(game);
	exit(EXIT_SUCCESS);
	return (0);
}
