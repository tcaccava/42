/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 16:39:13 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 10:24:57 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	error_message(char *message)
{
	ft_putstr_fd("\033[0;31m(This is bad)\n", STDOUT_FILENO);
	ft_putendl_fd(message, STDOUT_FILENO);
	exit(EXIT_FAILURE);
}

int	key_handler(int keycode, t_game *game)
{
	return (handle_movement(keycode, game));
}

int	main(int ac, char **av)
{
	char	**raw_map;
	t_game	game;

	validate_args(av, ac);
	raw_map = read_map(av[1]);
	if (!raw_map)
		error_message("Error reading map file!");
	validate_map(raw_map);
	validate_playability(raw_map);
	init_game_struct(&game, raw_map);
	printf("\n");
	start_game(&game);
	return (0);
}
