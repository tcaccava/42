/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path_items.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:43:35 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:43:37 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Verifies player can reach all collectibles without going through exit */
int	can_collect_all_items(t_game *game)
{
	t_flood_data	data;
	int				result;

	data.map = duplicate_map(game);
	if (!data.map)
		return (0);
	data.map[game->map.exit_pos.y][game->map.exit_pos.x] = '1';
	data.collectibles = 0;
	data.game = game;
	flood_fill_collect(&data, game->map.player_pos.x, game->map.player_pos.y);
	result = (data.collectibles == game->map.collectibles);
	free_temp_map(data.map, game->map.height);
	return (result);
}

/* Checks if player can reach the exit */
int	can_reach_exit(t_game *game)
{
	t_flood_data	data;
	int				result;

	data.map = duplicate_map(game);
	if (!data.map)
		return (0);
	data.exit_found = 0;
	data.game = game;
	flood_fill_exit(&data, game->map.player_pos.x, game->map.player_pos.y);
	result = data.exit_found;
	free_temp_map(data.map, game->map.height);
	return (result);
}
