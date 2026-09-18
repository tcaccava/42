/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collectibles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 04:02:47 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/03 13:56:29 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Handle collectibles collection
void handle_collectible(t_game *game, int new_x, int new_y)
{
	if (game->map[new_y][new_x] == 'C')
	{
		spawn_particles(game, new_x, new_y); // spawn particles before removing collectible
		game->map[new_y][new_x] = '0';		 // remove collectibles
		game->collectibles--;				 // update collectible count
		printf("Collectible collected, remaining : %d\n", game->collectibles);
	}
}

// Counts collectibles
void count_collectibles(t_game *game)
{
	int y;
	int x;

	game->collectibles = 0;
	y = 0;
	x = 0;
	if (!game || !game->map)
		return;
	while (y < game->height && game->map[y])
	{
		if (!game->map[y])
			return;
		x = 0;
		while (x < game->width && game->map[y][x])
		{
			if (game->map[y][x] == 'C')
				game->collectibles++;
			x++;
		}
		y++;
	}
}
