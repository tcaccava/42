/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:52:04 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 13:52:06 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

int	exit_game(t_game *game, char *error_msg, int exit_code)
{
	static int	error_printed = 0;

	if (error_msg && !error_printed)
	{
		error_printed = 1;
		ft_putendl_fd("Error", 2);
		ft_putendl_fd(error_msg, 2);
	}
	if (game->map.grid)
		free_map(game);
	if (game->mlx)
	{
		free_textures(game);
		if (game->win)
			mlx_destroy_window(game->mlx, game->win);
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	return (exit_code);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (exit_game(&game, "Wrong number of arguments", 1));
	if (!init_game(&game, argv[1]))
		return (1);
	mlx_hook(game.win, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.win, 17, 0, handle_window_close, &game);
	mlx_loop_hook(game.mlx, (int (*)(void *))render_game, &game);
	mlx_loop(game.mlx);
	return (0);
}
