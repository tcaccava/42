/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_player.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:00:38 by fionni            #+#    #+#             */
/*   Updated: 2026/02/05 21:00:38 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	sl_is_player(char c)
{
	return (c == 'P');
}

void	sl_set_player(t_game *g, int x, int y)
{
	g->player_x = x;
	g->player_y = y;
}

int	sl_scan_row_for_player(t_game *g, int y)
{
	int	x;

	x = 0;
	while (x < g->map.width)
	{
		if (sl_is_player(g->map.grid[y][x]))
		{
			sl_set_player(g, x, y);
			return (1);
		}
		x++;
	}
	return (0);
}

void	sl_find_player_position(t_game *g)
{
	int	y;

	y = 0;
	while (y < g->map.height)
	{
		if (sl_scan_row_for_player(g, y))
			return ;
		y++;
	}
}

void	sl_player_dummy(void)
{
	return ;
}
