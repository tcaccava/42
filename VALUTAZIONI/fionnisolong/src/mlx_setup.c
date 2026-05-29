/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:00:50 by fionni            #+#    #+#             */
/*   Updated: 2026/02/05 21:00:50 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	sl_init_mlx(t_game *g)
{
	g->mlx = mlx_init();
	if (!g->mlx)
		sl_error("MLX initialization failed");
}

void	sl_create_window(t_game *g)
{
	int	w;
	int	h;

	w = g->map.width * 64;
	h = g->map.height * 64;
	g->win = mlx_new_window(g->mlx, w, h, "so_long");
	if (!g->win)
		sl_error("Window creation failed");
}

void	sl_destroy_window(t_game *g)
{
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	g->win = NULL;
}

void	sl_setup_hooks(t_game *g)
{
	mlx_hook(g->win, 17, 0, sl_exit_game, g);
	mlx_key_hook(g->win, sl_key_hook, g);
}

void	sl_start_loop(t_game *g)
{
	sl_setup_hooks(g);
	mlx_loop(g->mlx);
}
