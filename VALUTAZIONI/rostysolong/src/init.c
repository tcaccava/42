/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:46:04 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:46:06 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int	init_game(t_game *game, char *map_path)
{
	ft_memset(game, 0, sizeof(t_game));
	game->moves = 0;
	game->collected = 0;
	game->cannot_collect_all = 0;
	if (!parse_map(game, map_path))
		return (0);
	if (!validate_map(game))
		return (0);
	find_exit_position(game);
	if (!init_mlx(game))
		return (0);
	if (!load_textures(game))
		return (0);
	return (1);
}

void	find_exit_position(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == 'E')
			{
				game->map.exit_pos.x = j;
				game->map.exit_pos.y = i;
				return ;
			}
			j++;
		}
		i++;
	}
}

int	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (exit_game(game, "MLX initialization failed", 0));
	game->win = mlx_new_window(game->mlx, game->map.width * TILE_SIZE,
			game->map.height * TILE_SIZE, "so_long");
	if (!game->win)
		return (exit_game(game, "Window creation failed", 0));
	return (1);
}

int	load_textures(t_game *game)
{
	game->wall.img = mlx_xpm_file_to_image(game->mlx, "textures/wall.xpm",
			&game->wall.width, &game->wall.height);
	game->floor.img = mlx_xpm_file_to_image(game->mlx, "textures/floor.xpm",
			&game->floor.width, &game->floor.height);
	game->player.img = mlx_xpm_file_to_image(game->mlx, "textures/player.xpm",
			&game->player.width, &game->player.height);
	game->collect.img = mlx_xpm_file_to_image(game->mlx, "textures/collect.xpm",
			&game->collect.width, &game->collect.height);
	game->exit.img = mlx_xpm_file_to_image(game->mlx, "textures/exit.xpm",
			&game->exit.width, &game->exit.height);
	if (!game->wall.img || !game->floor.img || !game->player.img
		|| !game->collect.img || !game->exit.img)
		return (exit_game(game, "Failed to load textures", 0));
	return (1);
}
