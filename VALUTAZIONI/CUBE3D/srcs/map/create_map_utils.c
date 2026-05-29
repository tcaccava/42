/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_map_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:20:27 by lonulli           #+#    #+#             */
/*   Updated: 2025/05/16 10:20:28 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "libft.h"
#include <string.h>

int		skip_white_spaces(char *map);
int		is_info(char *curr);
void	find_start_and_end_of_path(int *start, int *end, char *map_line);

void	extract_map_only(t_data *data)
{
	int		y;
	int		idx;
	char	**map;

	y = 0;
	idx = 0;
	map = data->map->map;
	while (map[y + 1] != NULL)
	{
		idx = skip_white_spaces(map[y]);
		if (is_info(&map[y][idx]) == 1)
			y++;
		else
			break ;
	}
	data->map->map_start = y;
	data->map->map_height -= data->map->map_start;
}

void	find_start_and_end_of_path(int *start, int *end, char *map_line)
{
	*start = 0;
	*end = 0;
	*start = skip_white_spaces(map_line);
	while (map_line && map_line[*start] && map_line[*start] >= 'A'
		&& map_line[*start] <= 'Z')
		(*start)++;
	while (map_line[*start] && is_space(map_line[*start]))
		(*start)++;
	*end = *start;
	while (map_line[*end] && map_line[*end] != '\n')
		(*end)++;
}

int	is_info(char *curr)
{
	if (ft_strncmp(curr, "NO ", 3) == 0 || ft_strncmp(curr, "N ", 2) == 0)
		return (1);
	if (ft_strncmp(curr, "SO ", 3) == 0 || ft_strncmp(curr, "S ", 2) == 0)
		return (1);
	if (ft_strncmp(curr, "WE ", 3) == 0 || ft_strncmp(curr, "W ", 2) == 0)
		return (1);
	if (ft_strncmp(curr, "EA ", 3) == 0 || ft_strncmp(curr, "E ", 2) == 0)
		return (1);
	if (ft_strncmp(curr, "F ", 2) == 0)
		return (1);
	if (ft_strncmp(curr, "C ", 2) == 0)
		return (1);
	if (ft_strlen(curr) == 0)
		return (1);
	return (0);
}

int	skip_white_spaces(char *map)
{
	int	y;

	y = 0;
	while (map[y] && is_space(map[y]) == 1)
		y++;
	return (y);
}

void	are_all_info_present(t_data *data)
{
	if (data->map->we_txt_path == NULL || data->map->so_txt_path == NULL
		|| data->map->no_txt_path == NULL || data->map->ea_txt_path == NULL
		|| data->map->ceiling_info == NULL || data->map->floor_info == NULL)
	{
		data->err_type = E_MISS_INFO;
		print_err_and_free(data, NULL);
	}
}
