/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 03:13:50 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/03 03:17:31 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	update_animation(t_game *game)
{
	game->anim_timer++;
	if (game->anim_timer >= 50)
	{
		game->current_frame++;
		if (game->current_frame >= 3)
			game->current_frame = 0;
		game->anim_timer = 0;
	}
}

int	update_game(t_game *game)
{
	update_animation(game);
	render_map(game);
	return (0);
}
