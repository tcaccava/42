/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_enemies_b.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 05:54:51 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 09:46:41 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

int	count_enemies(char **map)
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
			if (map[y][x] == 'N')
				count++;
			x++;
		}
		y++;
	}
	return (count);
}

void	find_all_enemies(t_game *game)
{
	int	x;
	int	y;
	int	enemy_index;

	enemy_index = 0;
	y = 0;
	while (game->map[y] && enemy_index < game->enemy_count)
	{
		x = 0;
		while (game->map[y][x] && enemy_index < game->enemy_count)
		{
			if (game->map[y][x] == 'N')
			{
				game->enemies[enemy_index].x = x;
				game->enemies[enemy_index].y = y;
				game->enemies[enemy_index].direction = 1;
				enemy_index++;
			}
			x++;
		}
		y++;
	}
}
