/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:47:05 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:47:06 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Processes line breaks in map content */
static void	process_line_break(t_game *game, int *width, int *line_count)
{
	if (game->map.width == 0)
		game->map.width = *width;
	else if (*width != game->map.width)
		return ;
	(*line_count)++;
	*width = 0;
}

/* Calculates map dimensions (width and height) */
void	get_map_dimensions(t_game *game, char *map_content)
{
	int	i;
	int	width;

	game->map.height = 0;
	game->map.width = 0;
	i = 0;
	width = 0;
	while (map_content[i])
	{
		if (map_content[i] == '\n')
			process_line_break(game, &width, &game->map.height);
		else
			width++;
		i++;
	}
	if (width > 0)
	{
		game->map.height++;
		if (game->map.width == 0)
			game->map.width = width;
	}
}

/* Allocates memory for the grid rows */
static char	**allocate_grid_rows(t_game *game)
{
	char	**grid;

	grid = (char **)malloc(sizeof(char *) * (game->map.height + 1));
	if (!grid)
		return (NULL);
	return (grid);
}

/* Fills a single row in the grid */
static int	fill_grid_row(char **grid, char *map_content, int i, t_game *game)
{
	int	j;
	int	k;

	grid[i] = (char *)malloc(sizeof(char) * (game->map.width + 1));
	if (!grid[i])
		return (0);
	j = 0;
	k = 0;
	while (j < game->map.width)
	{
		if (map_content[k] == '\n')
			k++;
		grid[i][j++] = map_content[k++];
	}
	grid[i][j] = '\0';
	return (k);
}

/* Creates a 2D grid from the map content string */
char	**create_map_grid(t_game *game, char *map_content)
{
	char	**grid;
	int		i;
	int		k;

	grid = allocate_grid_rows(game);
	if (!grid)
		return (NULL);
	i = 0;
	k = 0;
	while (i < game->map.height)
	{
		k += fill_grid_row(grid, map_content + k, i, game);
		if (!grid[i])
		{
			while (--i >= 0)
				free(grid[i]);
			free(grid);
			return (NULL);
		}
		i++;
	}
	grid[i] = NULL;
	return (grid);
}
