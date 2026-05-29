/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:45:41 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:45:44 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Handles keyboard input to control the game */
int	handle_keypress(int keycode, t_game *game)
{
	if (keycode == 65307)
		return (handle_window_close(game));
	else if (keycode == 119 || keycode == 65362)
		move_player(game, 0, -1);
	else if (keycode == 115 || keycode == 65364)
		move_player(game, 0, 1);
	else if (keycode == 97 || keycode == 65361)
		move_player(game, -1, 0);
	else if (keycode == 100 || keycode == 65363)
		move_player(game, 1, 0);
	return (0);
}

/* Handles window close event */
int	handle_window_close(t_game *game)
{
	exit_game(game, NULL, 0);
	exit(0);
	return (0);
}
