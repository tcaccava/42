/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies_move_b.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 05:05:05 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 07:56:35 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static int	check_wall_collision(t_game *game, t_enemy *enemy, int next_x)
{
	if (game->map[enemy->y][next_x] == '1'
		|| game->map[enemy->y][next_x] == 'E'
		|| game->map[enemy->y][next_x] == 'C')
		return (1);
	if (next_x <= 0 || next_x >= game->map_width - 1)
		return (1);
	return (0);
}

static int	check_enemy_collision(t_game *game, t_enemy *enemy, int next_x)
{
	int	i;

	i = 0;
	while (i < game->enemy_count)
	{
		if (&game->enemies[i] != enemy
			&& game->enemies[i].x == next_x
			&& game->enemies[i].y == enemy->y)
			return (1);
		else if (game->player_x == next_x && game->player_y == enemy->y)
		{
			game_over(game);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	move_single_enemy(t_game *game, t_enemy *enemy)
{
	int	next_x;

	next_x = enemy->x + enemy->direction;
	if (check_wall_collision(game, enemy, next_x)
		|| check_enemy_collision(game, enemy, next_x))
		enemy->direction *= -1;
	else
		enemy->x += enemy->direction;
}

void	move_enemies_patrol(t_game *game)
{
	int	i;

	if (!game->enemies)
		return ;
	i = 0;
	while (i < game->enemy_count)
	{
		move_single_enemy(game, &game->enemies[i]);
		i++;
	}
}
