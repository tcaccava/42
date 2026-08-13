/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 05:33:07 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/03 05:35:07 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	**create_map_copy(t_game *game)
{
	char	**copy;
	int		i;

	copy = malloc(sizeof(char *) * game->height);
	i = 0;
	while (i < game->height)
	{
		copy[i] = malloc(sizeof(char) * (game->width + 1));
		ft_strcpy(copy[i], game->map[i]);
		i++;
	}
	return (copy);
}

void	free_map_copy(char **map_copy, t_game *game)
{
	int	i;

	i = 0;
	while (i < game->height)
	{
		free(map_copy[i]);
		i++;
	}
	free(map_copy);
}

int	is_valid_move(int x, int y, t_game *game)
{
	if (!game || !game->map)
	{
		ft_printf("Error: game or game->map is NULL\n");
		return (0);
	}
	if (x < 0 || y < 0 || x >= game->width || y >= game->height
		|| game->map[y][x] == '1')
		return (0);
	return (x >= 0 && x < game->width && y >= 0 && y < game->height
		&& game->map[y][x] != '1');
}

int	find_path(int x, int y, t_game *game, char **map_copy)
{
	if (map_copy[y][x] == 'E')
		return (1);
	if (map_copy[y][x] == '1')
		return (0);
	map_copy[y][x] = '1';
	if (is_valid_move(x - 1, y, game) && find_path(x - 1, y, game, map_copy))
		return (1);
	if (is_valid_move(x + 1, y, game) && find_path(x + 1, y, game, map_copy))
		return (1);
	if (is_valid_move(x, y - 1, game) && find_path(x, y - 1, game, map_copy))
		return (1);
	if (is_valid_move(x, y + 1, game) && find_path(x, y + 1, game, map_copy))
		return (1);
	return (0);
}

int	is_path_valid(t_game *game)
{
	int		result;
	char	**map_copy;

	map_copy = create_map_copy(game);
	result = find_path(game->player_x, game->player_y, game, map_copy);
	free_map_copy(map_copy, game);
	return (result);
}
