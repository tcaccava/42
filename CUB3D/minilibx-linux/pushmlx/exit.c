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

int	close_game(t_game *game)
{
	cleanup(game);
	exit(0);
	return (0);
}

int	close_window(t_game *game)
{
	cleanup(game);
	exit(0);
	return (0);
}

void	destroy_image_if_not_null(void *mlx, void *img)
{
	if (img == NULL)
	{
		ft_printf("Warning: image is NULL.\n");
	}
	else if (img == (void *)1)
	{
		ft_printf("Warning: image has invalid value (0x1).\n");
	}
	if (img != NULL && img != (void *)1)
	{
		mlx_destroy_image(mlx, img);
	}
}

void	cleanup(t_game *game)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		destroy_image_if_not_null(game->mlx, game->player_sprites[i]);
		i++;
	}
	destroy_image_if_not_null(game->mlx, game->wall_img);
	destroy_image_if_not_null(game->mlx, game->collectibles_img);
	destroy_image_if_not_null(game->mlx, game->exit_img);
	destroy_image_if_not_null(game->mlx, game->empty_img);
	destroy_image_if_not_null(game->mlx, game->enemy_img);
	i = 0;
	while (i < game->height && game->map[i])
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
