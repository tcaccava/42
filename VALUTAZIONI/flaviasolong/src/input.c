/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:56:53 by faiello           #+#    #+#             */
/*   Updated: 2025/10/17 16:08:43 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdio.h>

static void	finalize_move(t_game *game, int nx, int ny, char tile)
{
	if (tile == 'C')
	{
		game->map.map[ny][nx] = '0';
		game->collected++;
		if (game->collected == game->map.collectibles)
			game->exit_is_open = true;
	}
	game->map.player.x = nx;
	game->map.player.y = ny;
	game->moves++;
	ft_printf("Moves: %d\n", game->moves);
}

static void	try_move(t_game *game, int dx, int dy)
{
	int		nx;
	int		ny;
	char	tile;

	nx = game->map.player.x + dx;
	ny = game->map.player.y + dy;
	if (nx < 0 || nx >= game->map.width || ny < 0 || ny >= game->map.height)
		return ;
	tile = game->map.map[ny][nx];
	if (tile == '1')
		return ;
	if (tile == 'E')
	{
		if (game->exit_is_open)
		{
			game->moves++;
			ft_printf("Moves: %d\n", game->moves);
			ft_printf("You win!\n");
			destroy_game(game);
			exit(EXIT_SUCCESS);
		}
		return ;
	}
	finalize_move(game, nx, ny, tile);
}

int	handle_key(int keycode, t_game *game)
{
	if (keycode == 65307)
		handle_close(game);
	else if (keycode == 'W' || keycode == 'w')
		try_move(game, 0, -1);
	else if (keycode == 'S' || keycode == 's')
		try_move(game, 0, 1);
	else if (keycode == 'A' || keycode == 'a')
		try_move(game, -1, 0);
	else if (keycode == 'D' || keycode == 'd')
		try_move(game, 1, 0);
	return (0);
}

int	handle_close(t_game *game)
{
	destroy_game(game);
	exit(EXIT_SUCCESS);
	return (0);
}
