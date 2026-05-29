/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:20:18 by lonulli           #+#    #+#             */
/*   Updated: 2025/05/16 10:20:20 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "libft.h"
#include <string.h>

static void	extract_map_infos(t_data *data, char **map, int x, int y);
static void	extract_path(t_data *data, char *path, char *which);
static void	extract_color(t_data *data, char *map_line, char which);

char	**fill_map(t_data *data)
{
	char	**map;
	char	*res;

	data->map->map_width = 0;
	map = ft_calloc(1, sizeof(char *));
	if (!map)
	{
		data->err_type = E_MAL_FAIL;
		print_err_and_free(data, NULL);
	}
	while (1)
	{
		res = get_next_line(data->map_ptr);
		if (!res)
			break ;
		data->map->map_height++;
		map = ft_strscat(map, res);
		if (!map)
		{
			data->err_type = E_MAL_FAIL;
			print_err_and_free(data, NULL);
			return (NULL);
		}
	}
	return (map);
}

void	get_map_infos(t_data *data)
{
	int		start;
	char	**map;
	int		idx;

	start = data->map->map_start;
	map = data->map->map;
	while (start >= 0 && map[start])
	{
		idx = skip_white_spaces(map[start]);
		extract_map_infos(data, map, idx, start);
		start--;
	}
	are_all_info_present(data);
}

void	extract_map_infos(t_data *data, char **map, int x, int y)
{
	if (!ft_strncmp(&map[y][x], "NO ", 3) || !ft_strncmp(&map[y][x], "N ", 2))
	{
		if (!data->map->no_txt_path)
			extract_path(data, map[y], "NO ");
	}
	if (!ft_strncmp(&map[y][x], "SO ", 3) || !ft_strncmp(&map[y][x], "S ", 2))
	{
		if (!data->map->so_txt_path)
			extract_path(data, map[y], "SO ");
	}
	if (!ft_strncmp(&map[y][x], "WE ", 3) || !ft_strncmp(&map[y][x], "W ", 2))
	{
		if (!data->map->we_txt_path)
			extract_path(data, map[y], "WE ");
	}
	if (!ft_strncmp(&map[y][x], "EA ", 3) || !ft_strncmp(&map[y][x], "E ", 2))
	{
		if (!data->map->ea_txt_path)
			extract_path(data, map[y], "EA ");
	}
	if (!ft_strncmp(&map[y][x], "F ", 2))
		extract_color(data, map[y], 'F');
	if (!ft_strncmp(&map[y][x], "C ", 2))
		extract_color(data, map[y], 'C');
}

void	extract_path(t_data *data, char *map_line, char *which)
{
	int	start;
	int	end;

	find_start_and_end_of_path(&start, &end, map_line);
	if (!data->map->ea_txt_path && ft_strncmp(which, "EA ", 3) == 0)
	{
		data->map->ea_txt_path = ft_calloc(1, ft_strlen(map_line + 1));
		ft_memmove(data->map->ea_txt_path, &map_line[start], end - start);
	}
	if (!data->map->we_txt_path && ft_strncmp(which, "WE ", 3) == 0)
	{
		data->map->we_txt_path = ft_calloc(1, ft_strlen(map_line + 1));
		ft_memmove(data->map->we_txt_path, &map_line[start], end - start);
	}
	if (!data->map->so_txt_path && ft_strncmp(which, "SO ", 3) == 0)
	{
		data->map->so_txt_path = ft_calloc(1, ft_strlen(map_line + 1));
		ft_memmove(data->map->so_txt_path, &map_line[start], end - start);
	}
	if (!data->map->no_txt_path && ft_strncmp(which, "NO ", 3) == 0)
	{
		data->map->no_txt_path = ft_calloc(1, ft_strlen(map_line + 1));
		ft_memmove(data->map->no_txt_path, &map_line[start], end - start);
	}
}

static void	extract_color(t_data *data, char *map_line, char which)
{
	int	idx;

	idx = 0;
	while (map_line[idx] && is_space(map_line[idx]))
		idx++;
	while (map_line[idx] && (map_line[idx] == 'F' || map_line[idx] == 'C'))
		idx++;
	while (map_line[idx] && is_space(map_line[idx]))
		idx++;
	if (!data->map->ceiling_info && which == 'C')
	{
		data->map->ceiling_info = ft_calloc(1, ft_strlen(map_line) + 1);
		ft_memmove(data->map->ceiling_info, &map_line[idx], ft_strlen(map_line)
			- idx);
	}
	else if (!data->map->floor_info && which == 'F')
	{
		data->map->floor_info = ft_calloc(1, ft_strlen(map_line) + 1);
		ft_memmove(data->map->floor_info, &map_line[idx], ft_strlen(map_line)
			- idx);
	}
}
