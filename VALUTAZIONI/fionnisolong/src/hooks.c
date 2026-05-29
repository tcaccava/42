/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:00:10 by fionni            #+#    #+#             */
/*   Updated: 2026/02/05 21:00:10 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	sl_close_hook(t_game *g)
{
	sl_exit_game(g);
	return (0);
}

int	sl_is_direction_key(int key)
{
	return (key == 'w' || key == 'a' || key == 's' || key == 'd'
		|| key == 'W' || key == 'A' || key == 'S' || key == 'D');
}

void	sl_handle_direction(t_game *g, int key)
{
	if (key == 'w' || key == 'W')
		sl_move_up(g);
	else if (key == 'a' || key == 'A')
		sl_move_left(g);
	else if (key == 's' || key == 'S')
		sl_move_down(g);
	else if (key == 'd' || key == 'D')
		sl_move_right(g);
}

int	sl_key_hook(int key, t_game *g)
{
	if (key == 65307)
		sl_exit_game(g);
	if (sl_is_direction_key(key))
	{
		sl_handle_direction(g, key);
		sl_refresh_window(g);
	}
	return (0);
}

void	sl_install_hooks(t_game *g)
{
	mlx_hook(g->win, 17, 0, sl_close_hook, g);
	mlx_key_hook(g->win, sl_key_hook, g);
}
