/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:57:23 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 13:57:26 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

void	check_collectible(t_game *game, int x, int y)
{
	if (game->map.grid[y][x] == 'C')
	{
		game->collected++;
		game->map.grid[y][x] = '0';
	}
}

void	check_exit(t_game *game)
{
	if (game->map.player_pos.x == game->map.exit_pos.x
		&& game->map.player_pos.y == game->map.exit_pos.y
		&& game->collected == game->map.collectibles)
	{
		ft_putstr_fd("You won with ", 1);
		ft_putnbr_fd(game->moves, 1);
		ft_putstr_fd(" moves!\n", 1);
		handle_window_close(game);
	}
}
