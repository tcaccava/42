/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 16:12:32 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 10:29:59 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	is_valid_move(t_game *game, int new_x, int new_y)
{
	if (new_x < 0 || new_x >= game->map_width)
		return (0);
	if (new_y < 0 || new_y >= game->map_height)
		return (0);
	if (game->map[new_y][new_x] == '1')
		return (0);
	if (game->map[new_y][new_x] == 'E' && game->collectibles > 0)
		return (0);
	return (1);
}

static void	handle_tile_interaction(t_game *game, int x, int y)
{
	char	tile;

	tile = game->map[y][x];
	if (tile == 'C')
	{
		game->map[y][x] = '0';
		game->collectibles--;
	}
	else if (tile == 'E')
	{
		if (game->collectibles == 0)
		{
			ft_putendl_fd("\033[1;32mCongratulations! You won!", STDOUT_FILENO);
			close_game(game);
		}
	}
}

static void	update_player_position(t_game *game, int new_x, int new_y)
{
	game->map[game->player_y][game->player_x] = '0';
	game->player_x = new_x;
	game->player_y = new_y;
	game->map[new_y][new_x] = 'P';
	game->moves++;
	ft_putstr_fd("\rMoves: ", STDOUT_FILENO);
	ft_putnbr_fd(game->moves, STDOUT_FILENO);
	ft_putstr_fd("    ", STDOUT_FILENO);
}

static void	move_player(t_game *game, int dx, int dy)
{
	int	new_x;
	int	new_y;

	new_x = game->player_x + dx;
	new_y = game->player_y + dy;
	if (is_valid_move(game, new_x, new_y))
	{
		handle_tile_interaction(game, new_x, new_y);
		update_player_position(game, new_x, new_y);
		render_map(game);
	}
}

int	handle_movement(int keycode, t_game *game)
{
	if (keycode == ESC_KEY)
		close_game(game);
	else if (keycode == W_KEY || keycode == UP_KEY)
		move_player(game, 0, -1);
	else if (keycode == S_KEY || keycode == DO_KEY)
		move_player(game, 0, 1);
	else if (keycode == A_KEY || keycode == LO_KEY)
		move_player(game, -1, 0);
	else if (keycode == D_KEY || keycode == RO_KEY)
		move_player(game, 1, 0);
	return (0);
}
