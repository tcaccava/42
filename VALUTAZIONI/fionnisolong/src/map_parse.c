/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:00:33 by fionni            #+#    #+#             */
/*   Updated: 2026/02/05 21:00:33 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	**sl_split_map(char *raw)
{
	char	**grid;
	int		i;
	int		len;

	grid = ft_split(raw, '\n');
	if (!grid)
		sl_error("Map split failed");
	i = 0;
	while (grid[i])
	{
		len = ft_strlen(grid[i]);
		if (len > 0 && grid[i][len - 1] == '\r')
			grid[i][len - 1] = '\0';
		i++;
	}
	return (grid);
}

int	sl_count_height(char **grid)
{
	int	h;

	h = 0;
	while (grid[h])
		h++;
	return (h);
}

int	sl_count_width(char **grid)
{
	int	w;

	w = ft_strlen(grid[0]);
	return (w);
}

void	sl_fill_map_struct(t_map *map, char **grid)
{
	map->grid = grid;
	map->height = sl_count_height(grid);
	map->width = sl_count_width(grid);
	map->players = 0;
	map->exits = 0;
	map->collectibles = 0;
}

void	sl_parse_map(t_map *map, char *file)
{
	char	*raw;
	char	**grid;

	raw = sl_load_raw_map(file);
	grid = sl_split_map(raw);
	free(raw);
	sl_fill_map_struct(map, grid);
}
