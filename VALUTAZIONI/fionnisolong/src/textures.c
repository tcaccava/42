/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:01:05 by fionni            #+#    #+#             */
/*   Updated: 2026/02/05 21:01:05 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	*sl_load_xpm(t_game *g, char *path)
{
	int		w;
	int		h;
	void	*img;

	w = 64;
	h = 64;
	img = mlx_xpm_file_to_image(g->mlx, path, &w, &h);
	return (img);
}

void	sl_verify_texture(void *img, char *name)
{
	if (!img)
		sl_error(name);
}

void	sl_load_all_textures(t_game *g)
{
	g->img.player = sl_load_xpm(g, "assets/player.xpm");
	g->img.wall = sl_load_xpm(g, "assets/wall.xpm");
	g->img.exit = sl_load_xpm(g, "assets/exit.xpm");
	g->img.collectible = sl_load_xpm(g, "assets/collectible.xpm");
	g->img.floor = sl_load_xpm(g, "assets/floor.xpm");
	sl_verify_texture(g->img.player, "Failed to load player.xpm");
	sl_verify_texture(g->img.wall, "Failed to load wall.xpm");
	sl_verify_texture(g->img.exit, "Failed to load exit.xpm");
	sl_verify_texture(g->img.collectible, "Failed to load collectible.xpm");
	sl_verify_texture(g->img.floor, "Failed to load floor.xpm");
}

void	sl_destroy_texture(t_game *g, void *img)
{
	if (img)
		mlx_destroy_image(g->mlx, img);
}

void	sl_destroy_all_textures(t_game *g)
{
	sl_destroy_texture(g, g->img.player);
	sl_destroy_texture(g, g->img.wall);
	sl_destroy_texture(g, g->img.exit);
	sl_destroy_texture(g, g->img.collectible);
	sl_destroy_texture(g, g->img.floor);
}
