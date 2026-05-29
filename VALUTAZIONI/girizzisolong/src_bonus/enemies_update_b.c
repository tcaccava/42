/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_update_b.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 06:28:44 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 06:32:24 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static void	clear_enemies_from_map(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			if (game->map[y][x] == 'N')
				game->map[y][x] = '0';
			x++;
		}
		y++;
	}
}

static void	place_enemies_on_map(t_game *game)
{
	int	i;
	int	x;
	int	y;

	i = 0;
	while (i < game->enemy_count)
	{
		x = game->enemies[i].x;
		y = game->enemies[i].y;
		if (game->map[y][x] != 'P')
			game->map[y][x] = 'N';
		i++;
	}
}

void	update_enemies_on_map(t_game *game)
{
	if (!game->enemies)
		return ;
	clear_enemies_from_map(game);
	place_enemies_on_map(game);
}

void	free_enemies(t_game *game)
{
	if (game->enemies)
	{
		free(game->enemies);
		game->enemies = NULL;
	}
}
