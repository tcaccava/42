/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: [your_name] <[your_email]>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:00:00 by [your_name]      #+#    #+#             */
/*   Updated: 2025/04/29 12:00:00 by [your_name]     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Counts map components and saves player and exit positions */
static void	count_components(t_game *game, int i, int j)
{
	if (game->map.grid[i][j] == 'C')
		game->map.collectibles++;
	else if (game->map.grid[i][j] == 'E')
	{
		game->map.exits++;
		game->map.exit_pos.x = j;
		game->map.exit_pos.y = i;
	}
	else if (game->map.grid[i][j] == 'P')
	{
		game->map.players++;
		game->map.player_pos.x = j;
		game->map.player_pos.y = i;
	}
	else if (game->map.grid[i][j] != '0' && game->map.grid[i][j] != '1')
		game->invalid_char = 1;
}

/* Validates required map components (collectibles, exit, player) */
static int	validate_component_counts(t_game *game)
{
	if (game->invalid_char)
		return (exit_game(game, "Wrong characters", 0));
	if (game->map.collectibles == 0)
		return (exit_game(game, "No object", 0));
	if (game->map.exits == 0)
		return (exit_game(game, "No exit", 0));
	if (game->map.exits > 1)
		return (exit_game(game, "Duplicate exit", 0));
	if (game->map.players == 0)
		return (exit_game(game, "No player", 0));
	if (game->map.players > 1)
		return (exit_game(game, "Duplicate player", 0));
	return (1);
}

/* Checks if map contains required components in correct quantities */
int	check_components(t_game *game)
{
	int	i;
	int	j;

	game->map.collectibles = 0;
	game->map.exits = 0;
	game->map.players = 0;
	game->invalid_char = 0;
	i = -1;
	while (++i < game->map.height)
	{
		j = -1;
		while (++j < game->map.width)
			count_components(game, i, j);
	}
	return (validate_component_counts(game));
}
