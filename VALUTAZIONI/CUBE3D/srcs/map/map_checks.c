/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:20:32 by lonulli           #+#    #+#             */
/*   Updated: 2025/05/16 10:20:33 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "libft.h"
#include <string.h>

static void	check_zeros_and_doors(t_data *data, char **map, int x, int y);
static void	check_player_existence(t_data *data);

void	check_format(char *arg)
{
	int	i;

	i = 0;
	while (arg && arg[i])
		i++;
	while (i > 0)
	{
		if (arg[i] == '.')
			break ;
		i--;
	}
	if (ft_strncmp(&arg[i], ".cub", 5))
	{
		printf("Error: Format not valid\n");
		exit(1);
	}
}

void	is_map_closed(t_data *data)
{
	int		x;
	int		y;
	char	**map;
	int		max_width;

	y = data->map->map_start;
	map = data->map->map;
	max_width = 0;
	while (map[y])
	{
		x = 0;
		while (map[y][x])
		{
			if (max_width < (int)ft_strlen(map[y]))
				max_width = ft_strlen(map[y]) - 1;
			check_zeros_and_doors(data, map, x, y);
			x++;
		}
		y++;
	}
	data->map->map_end = y;
	data->map->map_width = max_width;
}

void	is_map_valid(t_data *data)
{
	char	**map;
	int		y;
	int		x;

	y = data->map->map_start;
	x = 0;
	map = data->map->map;
	while (map[y] && data->map->is_map_valid)
	{
		while (map[y][x])
		{
			check_multiple_nl(data, map, x, y);
			if (check_and_set(data, map[y][x], x, y) == 0)
			{
				data->err_type = E_INV_MAP;
				print_err_and_free(data, NULL);
			}
			x++;
		}
		x ^= x;
		y++;
	}
	check_player_existence(data);
}

static void	check_player_existence(t_data *data)
{
	if (data->player->exists == 0 || data->player->exists > 1)
	{
		data->err_type = E_PCOUNT;
		print_err_and_free(data, NULL);
	}
}

static void	check_zeros_and_doors(t_data *data, char **map, int x, int y)
{
	if ((map[y][x] == '0') && check_zero_surroundings(data, map, x, y) == 0)
	{
		data->err_type = E_MAP_CLOSED;
		print_err_and_free(data, NULL);
	}
	if ((map[y][x] == 'D') && check_zero_surroundings(data, map, x, y) == 0)
	{
		data->err_type = E_DOOR;
		print_err_and_free(data, NULL);
	}
	if ((map[y][x] == 'P') && check_zero_surroundings(data, map, x, y) == 0)
	{
		data->err_type = E_DOOR;
		print_err_and_free(data, NULL);
	}
}
