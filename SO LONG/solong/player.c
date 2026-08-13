/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 05:04:30 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/06 02:31:42 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	find_player_position(t_game *game, char *line, int row)
{
	int	j;

	j = 0;
	while (line[j])
	{
		if (line[j] == 'P')
		{
			game->player_x = j;
			game->player_y = row;
		}
		j++;
	}
}

void	update_position(t_game *game, int new_x, int new_y)
{
	if (game->map[new_y][new_x] == 'E')
	{
		if (game->collectibles > 0)
		{
			ft_printf("You must collect all items first!. Remaining:%d\n",
				game->collectibles);
			return ;
		}
		else
		{
			ft_printf("Player wins! Total moves: %d\n", game->moves);
			mlx_loop_end(game->mlx);
			cleanup(game);
			return ;
		}
	}
	game->map[game->player_y][game->player_x] = '0';
	game->player_x = new_x;
	game->player_y = new_y;
	game->map[game->player_y][game->player_x] = 'P';
}

void	move_player(t_game *game, int new_x, int new_y)
{
	if (game->map[new_y][new_x] != '1')
	{
		if (game->map[new_y][new_x] == 'N')
		{
			ft_printf("Game Over! You were caught by an enemy!\n");
			close_game(game);
		}
		count_collectibles(game);
		handle_collectible(game, new_x, new_y);
		update_position(game, new_x, new_y);
		game->moves++;
		ft_printf("Moves: %d\n", game->moves);
		render_map(game);
		display_moves(game);
	}
}
