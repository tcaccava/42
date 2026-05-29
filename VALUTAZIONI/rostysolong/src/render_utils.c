/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:47:45 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:47:50 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Displays the item counter in the window */
static void	display_items_counter(t_game *game, char *collect_str)
{
	char	*max_collect;

	max_collect = ft_itoa(game->map.collectibles);
	if (max_collect)
	{
		mlx_string_put(game->mlx, game->win, 120, 20, 0xFFFFFF, "Items: ");
		mlx_string_put(game->mlx, game->win, 170, 20,
			0xFFFFFF, collect_str);
		mlx_string_put(game->mlx, game->win, 185, 20, 0xFFFFFF, "/");
		mlx_string_put(game->mlx, game->win, 195, 20,
			0xFFFFFF, max_collect);
		free(max_collect);
	}
}

/* Updates the game window with current stats */
void	update_window(t_game *game)
{
	char	*moves_str;
	char	*collect_str;

	moves_str = ft_itoa(game->moves);
	if (moves_str)
	{
		mlx_string_put(game->mlx, game->win, 10, 20, 0xFFFFFF, "Moves: ");
		mlx_string_put(game->mlx, game->win, 60, 20, 0xFFFFFF, moves_str);
		free(moves_str);
	}
	collect_str = ft_itoa(game->collected);
	if (collect_str)
	{
		display_items_counter(game, collect_str);
		free(collect_str);
	}
}
