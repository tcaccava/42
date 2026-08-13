/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 04:04:07 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/05 23:05:16 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	load_enemies(t_game *game)
{
	int	x;
	int	y;
	int	enemy_count;

	x = 0;
	y = 0;
	enemy_count = 0;
	while (y < game->height)
	{
		x = 0;
		while (x < game->width)
		{
			if (game->map[y][x] == 'N')
			{
				game->enemies[enemy_count].x = x;
				game->enemies[enemy_count].y = y;
				enemy_count++;
			}
			x++;
		}
		y++;
	}
	game->num_enemies = enemy_count;
}

void	check_for_enemy(t_game *game, int new_x, int new_y)
{
	int	i;

	i = 0;
	while (i < game->num_enemies)
	{
		if (game->enemies[i].x == new_x && game->enemies[i].y == new_y)
		{
			ft_printf("Game Over! You hit an enemy.\n");
			cleanup(game);
			exit(0);
		}
		i++;
	}
}
