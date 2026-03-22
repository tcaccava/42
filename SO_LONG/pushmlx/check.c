/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 05:09:59 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/06 03:31:57 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	check_duplicates(t_game *game)
{
	int	player_count;
	int	exit_count;
	int	collectible_count;

	count_elements(game, &player_count, &exit_count, &collectible_count);
	if (player_count != 1)
	{
		ft_printf("Error: There must be exactly one player ('P').\n");
		cleanup(game);
		exit(1);
	}
	if (exit_count != 1)
	{
		ft_printf("Error: There must be exactly one exit position ('E').\n");
		cleanup(game);
		exit(1);
	}
	if (collectible_count < 1)
	{
		ft_printf("Error: There must be at least one collectible ('C').\n");
		cleanup(game);
		exit(1);
	}
}

void	check_map_shape(t_game *game)
{
	size_t	row_length;
	int		i;

	row_length = ft_strlen(game->map[0]);
	if ((int)row_length == game->height)
	{
		ft_printf("Error: The map cannot be square.\n");
		cleanup(game);
		exit(1);
	}
	i = 1;
	while (i < game->height)
	{
		if (ft_strlen(game->map[i]) != row_length)
		{
			ft_printf("Error: The map is not rectangular.\n");
			cleanup(game);
			exit(1);
		}
		i++;
	}
}

void	check_walls(t_game *game)
{
	int	x;
	int	y;

	x = 0;
	while (x < game->width)
	{
		if (game->map[0][x] != '1' || game->map[game->height - 1][x] != '1')
		{
			ft_printf("Error: Map is not closed at column %d\n", x);
			cleanup(game);
			exit(1);
		}
		x++;
	}
	y = 0;
	while (y < game->height)
	{
		if (game->map[y][0] != '1' || game->map[y][game->width - 1] != '1')
		{
			ft_printf("Error: Map is not closed at row %d\n", y);
			cleanup(game);
			exit(1);
		}
		y++;
	}
}

void	count_elements(t_game *game, int *player_count, int *exit_count,
		int *collectible_count)
{
	int	i;
	int	j;

	*player_count = 0;
	*exit_count = 0;
	*collectible_count = 0;
	i = 0;
	while (i < game->height)
	{
		j = 0;
		while (j < game->width)
		{
			if (game->map[i][j] == 'P')
				(*player_count)++;
			else if (game->map[i][j] == 'E')
				(*exit_count)++;
			else if (game->map[i][j] == 'C')
				(*collectible_count)++;
			j++;
		}
		i++;
	}
}

int	is_valid_map_extension(char *f)
{
	int	i;
	int	dot_count;

	i = 0;
	dot_count = 0;
	while (f[i] != '\0')
	{
		i++;
	}
	if (f[i - 4] == '.' && f[i - 3] == 'b' && f[i - 2] == 'e' && f[i
			- 1] == 'r')
	{
		i = 0;
		while (f[i] != '\0')
		{
			if (f[i] == '.' && f[i + 1] == 'b' && f[i + 2] == 'e' && f[i
					+ 3] == 'r')
				dot_count++;
			i++;
		}
		if (dot_count > 1)
			return (0);
		return (1);
	}
	return (0);
}
