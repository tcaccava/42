/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_animation_b.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 00:34:27 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 09:51:27 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	init_player_animations(t_game *game)
{
	load_idle_frames(game);
	game->images.player_anim.current_frame = 0;
	game->images.player_anim.last_frame_time = get_current_time_microseconds();
}

void	update_player_animation(t_game *game)
{
	long	current_time;
	long	time_diff;

	current_time = get_current_time_microseconds();
	time_diff = current_time - game->images.player_anim.last_frame_time;
	if (time_diff >= IDLE_ANIMATION_SPEED)
	{
		game->images.player_anim.current_frame++;
		if (game->images.player_anim.current_frame >= IDLE_FRAMES)
			game->images.player_anim.current_frame = 0;
		game->images.player_anim.last_frame_time = current_time;
	}
}

void	*get_current_player_sprite(t_game *game)
{
	int		frame;
	void	*sprite;

	frame = game->images.player_anim.current_frame;
	if (frame >= IDLE_FRAMES)
		frame = 0;
	sprite = game->images.player_anim.idle_frames[frame];
	if (!sprite && game->images.player_anim.idle_frames[0])
		sprite = game->images.player_anim.idle_frames[0];
	return (sprite);
}

void	load_idle_frames(t_game *game)
{
	int	width;
	int	height;

	game->images.player_anim.idle_frames[0] = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/player_idle_1.xpm", &width, &height);
	game->images.player_anim.idle_frames[1] = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/player_idle_2.xpm", &width, &height);
	game->images.player_anim.idle_frames[2] = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/player_idle_3.xpm", &width, &height);
	game->images.player_anim.idle_frames[3] = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/player_idle_4.xpm", &width, &height);
	game->images.player_anim.idle_frames[4] = mlx_xpm_file_to_image(game->mlx,
			"sprites_bonus/player_idle_5.xpm", &width, &height);
}
