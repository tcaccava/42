/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: [your_name] <[your_email]>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:00:00 by [your_name]      #+#    #+#             */
/*   Updated: 2025/04/30 13:49:50 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Validates the map checking dimensions, walls and components */
int	validate_map(t_game *game)
{
	if (!check_rectangular(game))
		return (exit_game(game, "No rectangular", 0));
	if (!check_walls(game))
		return (exit_game(game, "Not surrounded by walls", 0));
	if (!check_components(game))
		return (0);
	if (!check_valid_path(game))
	{
		if (game->cannot_collect_all)
			return (exit_game(game, "Cannot collect all items", 0));
		else
			return (exit_game(game, "Duplicate exit", 0));
	}
	return (1);
}

/* Checks if the map is rectangular */
int	check_rectangular(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->map.height)
	{
		if (ft_strlen(game->map.grid[i]) != (size_t)game->map.width)
			return (0);
		i++;
	}
	if (game->map.width < 3 || game->map.height < 3)
		return (0);
	return (1);
}

/* Checks if the map is surrounded by walls */
int	check_walls(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if ((i == 0 || i == game->map.height - 1
					|| j == 0 || j == game->map.width - 1)
				&& game->map.grid[i][j] != '1')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
