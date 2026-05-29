/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_b.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 16:49:12 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 09:47:39 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

void	display_moves(t_game *game)
{
	char	*moves_str;
	char	*moves_num;

	moves_num = ft_itoa(game->moves);
	if (!moves_num)
		return ;
	moves_str = ft_strjoin("Moves: ", moves_num);
	free(moves_num);
	if (!moves_str)
		return ;
	mlx_string_put(game->mlx, game->win, 10, 20, 0x000000, moves_str);
	free(moves_str);
}

static void	render_tile(t_game *game, int x, int y, char tile)
{
	void	*image;

	mlx_put_image_to_window(game->mlx, game->win, game->images.floor,
		x * TILE_SIZE, y * TILE_SIZE);
	if (tile == '1')
		image = game->images.wall;
	else if (tile == 'P')
		image = get_current_player_sprite(game);
	else if (tile == 'C')
		image = game->images.collectible;
	else if (tile == 'E')
		image = game->images.exit;
	else if (tile == 'N')
		image = get_current_enemy_sprite(game);
	else if (tile == 'G')
		image = game->images.game_over;
	else
		return ;
	mlx_put_image_to_window(game->mlx, game->win, image,
		x * TILE_SIZE, y * TILE_SIZE);
}

void	render_map(t_game *game)
{
	int	x;
	int	y;

	if (!game->is_game_over)
	{
		update_player_animation(game);
		update_enemy_animation(game);
	}
	y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			render_tile(game, x, y, game->map[y][x]);
			x++;
		}
		y++;
	}
	display_moves(game);
}
