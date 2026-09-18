/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 04:06:00 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/06 03:06:45 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Closes game when pressing x button on the game window
int close_window(t_game *game)
{
	cleanup(game);
	exit(0);
	return (0);
}

// Checks images existence before destroying it
void destroy_image_if_not_null(void *mlx, void *img)
{
	if (img == NULL)
		ft_printf("Warning: image is NULL.\n");
	else if (img == (void *)1)
		ft_printf("Warning: image has invalid value (0x1).\n");
	if (img != NULL && img != (void *)1)
		mlx_destroy_image(mlx, img);
}

// Destroy everything to avoid leaks
void cleanup(t_game *game)
{
	int i;

	i = 0;
	while (i < 8) // player,enemies
	{
		destroy_image_if_not_null(game->mlx, game->player_sprites[i]);
		destroy_image_if_not_null(game->mlx, game->player_left_sprites[i]);
		destroy_image_if_not_null(game->mlx, game->enemy_left_sprites[i]);
		destroy_image_if_not_null(game->mlx, game->enemy_right_sprites[i]);
		i++;
	}
	while (i < 6) // trees
	{
		destroy_image_if_not_null(game->mlx, game->tree_sprites[i]);
		i++;
	}
	// static sprites
	destroy_image_if_not_null(game->mlx, game->wall_img);
	destroy_image_if_not_null(game->mlx, game->house_img);
	destroy_image_if_not_null(game->mlx, game->tower_img);
	destroy_image_if_not_null(game->mlx, game->collectibles_img);
	destroy_image_if_not_null(game->mlx, game->exit_img_left);
	destroy_image_if_not_null(game->mlx, game->exit_img_right);
	destroy_image_if_not_null(game->mlx, game->empty_img);
	destroy_image_if_not_null(game->mlx, game->wall_top_img);
	destroy_image_if_not_null(game->mlx, game->wall_bottom_img);
	destroy_image_if_not_null(game->mlx, game->wall_left_img);
	destroy_image_if_not_null(game->mlx, game->wall_right_img);
	destroy_image_if_not_null(game->mlx, game->wall_topleft_img);
	destroy_image_if_not_null(game->mlx, game->wall_topright_img);
	destroy_image_if_not_null(game->mlx, game->wall_bottomleft_img);
	destroy_image_if_not_null(game->mlx, game->wall_bottomright_img);

	i = 0;
	while (i < game->height && game->map[i]) // free every map row
	{
		free(game->map[i]);
		i++;
	}
	free(game->map);
	mlx_destroy_window(game->mlx, game->win);
	mlx_destroy_display(game->mlx);
	free(game->mlx);
	exit(0);
}
