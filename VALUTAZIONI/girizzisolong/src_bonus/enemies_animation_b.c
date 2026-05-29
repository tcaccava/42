/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_animation_b.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 04:06:38 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 09:50:29 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	init_enemy_animations(t_game *game)
{
	load_enemy_frames(game);
	game->images.enemy_anim.n_current_frame = 0;
	game->images.enemy_anim.n_last_frame_time = get_current_time_microseconds();
}

void	update_enemy_animation(t_game *game)
{
	long	current_time;
	long	time_diff;

	current_time = get_current_time_microseconds();
	time_diff = current_time - game->images.enemy_anim.n_last_frame_time;
	if (time_diff >= IDLE_ANIMATION_SPEED)
	{
		game->images.enemy_anim.n_current_frame++;
		if (game->images.enemy_anim.n_current_frame >= IDLE_FRAMES)
			game->images.enemy_anim.n_current_frame = 0;
		game->images.enemy_anim.n_last_frame_time = current_time;
	}
}

void	*get_current_enemy_sprite(t_game *game)
{
	int		frame;
	void	*sprite;

	frame = game->images.enemy_anim.n_current_frame;
	if (frame >= IDLE_FRAMES)
		frame = 0;
	sprite = game->images.enemy_anim.n_idle_frames[frame];
	if (!sprite && game->images.enemy_anim.n_idle_frames[0])
		sprite = game->images.enemy_anim.n_idle_frames[0];
	return (sprite);
}

void	load_enemy_frames(t_game *game)
{
	int	width;
	int	height;

	game->images.enemy_anim.n_idle_frames[0] = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/enemy_1.xpm", &width, &height);
	game->images.enemy_anim.n_idle_frames[1] = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/enemy_2.xpm", &width, &height);
	game->images.enemy_anim.n_idle_frames[2] = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/enemy_3.xpm", &width, &height);
	game->images.enemy_anim.n_idle_frames[3] = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/enemy_4.xpm", &width, &height);
	game->images.enemy_anim.n_idle_frames[4] = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/enemy_5.xpm", &width, &height);
}
