/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:00:55 by fionni            #+#    #+#             */
/*   Updated: 2026/02/05 21:00:55 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	sl_move_to(t_game *g, int nx, int ny)
{
	char	tile;

	tile = g->map.grid[ny][nx];
	if (tile == '1')
		return ;
	if (tile == 'C')
		g->map.collectibles--;
	if (tile == 'E')
	{
		if (g->map.collectibles == 0)
			sl_exit_game(g);
		return ;
	}
	g->map.grid[g->player_y][g->player_x] = '0';
	g->map.grid[ny][nx] = 'P';
	g->player_x = nx;
	g->player_y = ny;
	g->moves++;
	ft_putnbr_fd(g->moves, 1);
	write(1, "\r", 1);
}

void	sl_move_up(t_game *g)
{
	int	nx;
	int	ny;

	nx = g->player_x;
	ny = g->player_y - 1;
	sl_move_to(g, nx, ny);
}

void	sl_move_down(t_game *g)
{
	int	nx;
	int	ny;

	nx = g->player_x;
	ny = g->player_y + 1;
	sl_move_to(g, nx, ny);
}

void	sl_move_left(t_game *g)
{
	int	nx;
	int	ny;

	nx = g->player_x - 1;
	ny = g->player_y;
	sl_move_to(g, nx, ny);
}

void	sl_move_right(t_game *g)
{
	int	nx;
	int	ny;

	nx = g->player_x + 1;
	ny = g->player_y;
	sl_move_to(g, nx, ny);
}
