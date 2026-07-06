/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 04:15:56 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/03 13:55:50 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	allocate_map(t_game *game, char *file)
{
	int	num_lines;

	num_lines = count_map_lines(file);
	if (num_lines <= 0)
		exit(1);
	game->map = (char **)malloc(sizeof(char *) * (num_lines + 1));
	if (!game->map)
		exit(1);
	game->moves = 0;
	game->height = num_lines;
}

void	fill_map(t_game *game, char *file)
{
	int		fd;
	int		i;
	char	*line;

	fd = open(file, O_RDONLY);
	i = 0;
	if (fd == -1)
		exit(1);
	line = get_next_line(fd);
	while (line != NULL && i < game->height)
	{
		remove_newline(line);
		game->map[i] = ft_strdup(line);
		if (!game->map[i])
			exit(1);
		find_player_position(game, line, i);
		free(line);
		i++;
		line = get_next_line(fd);
	}
	fill_remaining_rows(game, i);
	close(fd);
	game->width = ft_strlen(game->map[0]);
	count_collectibles(game);
}

int	count_map_lines(char *file)
{
	int		fd;
	char	*line;
	int		count;

	fd = open(file, O_RDONLY);
	count = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		free(line);
		count++;
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

int	render_map(t_game *game)
{
	int	y;

	if (!game || !game->map)
	{
		ft_printf("Error: game or game->map is NULL\n");
		return (1);
	}
	y = 0;
	while (y < game->height && game->map[y] != NULL)
	{
		render_map_row(game, y);
		y++;
	}
	display_moves(game);
	return (0);
}

void	render_tile(t_game *game, char tile, int x, int y)
{
	if (tile == '1')
		mlx_put_image_to_window(game->mlx, game->win, game->wall_img, x * T, y
			* T);
	else if (tile == 'C')
		mlx_put_image_to_window(game->mlx, game->win, game->collectibles_img, x
			* T, y * T);
	else if (tile == 'E')
		mlx_put_image_to_window(game->mlx, game->win, game->exit_img, x * T, y
			* T);
	else if (tile == '0')
		mlx_put_image_to_window(game->mlx, game->win, game->empty_img, x * T, y
			* T);
	else if (tile == 'P')
		mlx_put_image_to_window(game->mlx, game->win,
			game->player_sprites[game->current_frame], x * T, y * T);
	else if (tile == 'N')
		mlx_put_image_to_window(game->mlx, game->win, game->enemy_img, x * T, y
			* T);
}
