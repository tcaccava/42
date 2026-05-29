/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:37:47 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 13:37:49 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Handles win condition when player reaches exit with all collectibles */
static void	handle_win(t_game *game)
{
	game->moves++;
	ft_putstr_fd("Moves: ", 1);
	ft_putnbr_fd(game->moves, 1);
	ft_putchar_fd('\n', 1);
	ft_putstr_fd("You won with ", 1);
	ft_putnbr_fd(game->moves, 1);
	ft_putstr_fd(" moves!\n", 1);
	handle_window_close(game);
}

/* Handles collecting an item */
static void	collect_item(t_game *game)
{
	game->collected++;
	ft_putstr_fd("Collected: ", 1);
	ft_putnbr_fd(game->collected, 1);
	ft_putstr_fd("/", 1);
	ft_putnbr_fd(game->map.collectibles, 1);
	ft_putchar_fd('\n', 1);
}

/* Updates player position on the map */
static void	update_player_position(t_game *game, int new_x, int new_y,
int on_exit)
{
	if (on_exit)
		game->map.grid[game->map.player_pos.y][game->map.player_pos.x] = 'E';
	else
		game->map.grid[game->map.player_pos.y][game->map.player_pos.x] = '0';
	if (game->map.grid[new_y][new_x] == 'C')
		collect_item(game);
	game->map.player_pos.x = new_x;
	game->map.player_pos.y = new_y;
	game->map.grid[new_y][new_x] = 'P';
}

/* Moves the player in the specified direction */
void	move_player(t_game *game, int x, int y)
{
	int	new_x;
	int	new_y;
	int	on_exit;

	new_x = game->map.player_pos.x + x;
	new_y = game->map.player_pos.y + y;
	if (game->map.grid[new_y][new_x] == '1')
		return ;
	if (game->map.grid[new_y][new_x] == 'E')
	{
		if (game->collected != game->map.collectibles)
			return ;
		handle_win(game);
		return ;
	}
	game->moves++;
	ft_putstr_fd("Moves: ", 1);
	ft_putnbr_fd(game->moves, 1);
	ft_putchar_fd('\n', 1);
	on_exit = (game->map.grid[game->map.player_pos.y]
		[game->map.player_pos.x] == 'E');
	update_player_position(game, new_x, new_y, on_exit);
}
