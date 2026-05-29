/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:44:21 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:44:23 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Validates the complete path for gameplay */
int	check_valid_path(t_game *game)
{
	if (!is_valid_path(game))
	{
		if (game->cannot_collect_all)
			return (exit_game(game, "Cannot collect all items", 0));
		else
			return (exit_game(game, "Duplicate exit", 0));
	}
	return (1);
}

/* Checks if all items are collectible and exit is reachable */
int	is_valid_path(t_game *game)
{
	if (!can_collect_all_items(game))
	{
		game->cannot_collect_all = 1;
		return (0);
	}
	if (!can_reach_exit(game))
		return (0);
	return (1);
}
