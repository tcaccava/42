/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 16:09:10 by faiello           #+#    #+#             */
/*   Updated: 2025/10/17 16:12:52 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <fcntl.h>

static int	loop_hook(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	render_game(game);
	return (0);
}

static int	has_extension(const char *path, const char *ext)
{
	size_t	path_len;
	size_t	ext_len;

	path_len = ft_strlen(path);
	ext_len = ft_strlen(ext);
	if (path_len < ext_len)
		return (0);
	return (!ft_strncmp(path + (path_len - ext_len), ext, ext_len));
}

static void	prepare_game(int argc, char **argv, t_game *game)
{
	if (argc != 2)
	{
		ft_putendl_fd("Usage: ./so_long <map.ber>", 2);
		exit(1);
	}
	if (!has_extension(argv[1], ".ber"))
	{
		ft_putendl_fd("Error, no .ber extension", 2);
		exit(1);
	}
	ft_bzero(game, sizeof(t_game));
	if (!load_map(argv[1], &game->map))
		error_exit("Failed to load map", game);
	if (!validate_map(&game->map))
		error_exit("Invalid map", game);
	if (!check_map_path(&game->map))
		error_exit("Map has no valid path", game);
	if (!init_game(game, "so_long"))
		error_exit("Failed to initialize game", game);
}

static void	start_event_loop(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, handle_key, game);
	mlx_hook(game->win, 17, 0, handle_close, game);
	mlx_loop_hook(game->mlx, loop_hook, game);
	mlx_loop(game->mlx);
}

int	main(int argc, char **argv)
{
	t_game	game;

	prepare_game(argc, argv, &game);
	start_event_loop(&game);
	destroy_game(&game);
	return (0);
}
