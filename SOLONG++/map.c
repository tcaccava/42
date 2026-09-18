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

// Count maps rows
int count_map_lines(char *file)
{
	int fd;
	char *line;
	int count;

	fd = open(file, O_RDONLY);
	count = 0;
	while ((line = get_next_line(fd)) != NULL)
	{
		free(line);
		count++;
	}
	close(fd);
	return (count);
}

// Mallocs map
void allocate_map(t_game *game, char *file)
{
	int num_lines;

	num_lines = count_map_lines(file);
	if (num_lines <= 0)
		exit(1);
	game->map = (char **)malloc(sizeof(char *) * (num_lines + 1));
	if (!game->map)
		exit(1);
	game->height = num_lines;
}

void fill_remaining_rows(t_game *game, int i)
{
	while (i < game->height)
	{
		game->map[i] = NULL;
		i++;
	}
}

// Build game map
void fill_map(t_game *game, char *file)
{
	int fd;
	int i;
	char *line;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		exit(1);

	i = 0;
	while (i < game->height && (line = get_next_line(fd)) != NULL)
	{
		remove_newline(line);
		game->map[i] = ft_strdup(line);
		if (!game->map[i])
			exit(1);
		find_player_position(game, line, i);
		free(line);
		i++;
	}
	fill_remaining_rows(game, i);
	close(fd);
	game->width = ft_strlen(game->map[0]);
	count_collectibles(game);
}

// Renders single tiles
void render_tile(t_game *game, char tile, int x, int y)
{
	if (tile == '1') // walls
	{
		if (x == 0 && y == 0)
			mlx_put_image_to_window(game->mlx, game->win, game->wall_topleft_img, x * T, y * T);
		else if (x == game->width - 1 && y == 0)
			mlx_put_image_to_window(game->mlx, game->win, game->wall_topright_img, x * T, y * T);
		else if (x == 0 && y == game->height - 1)
			mlx_put_image_to_window(game->mlx, game->win, game->wall_bottomleft_img, x * T, y * T);
		else if (x == game->width - 1 && y == game->height - 1)
			mlx_put_image_to_window(game->mlx, game->win, game->wall_bottomright_img, x * T, y * T);
		else if (y == 0)
			mlx_put_image_to_window(game->mlx, game->win, game->wall_top_img, x * T, y * T);
		else if (y == game->height - 1)
			mlx_put_image_to_window(game->mlx, game->win, game->wall_bottom_img, x * T, y * T);
		else if (x == 0)
			mlx_put_image_to_window(game->mlx, game->win, game->wall_left_img, x * T, y * T);
		else if (x == game->width - 1)
			mlx_put_image_to_window(game->mlx, game->win, game->wall_right_img, x * T, y * T);
		else
			mlx_put_image_to_window(game->mlx, game->win, game->wall_img, x * T, y * T);
	}
	else if (tile == 'C') // collectibles
		mlx_put_image_to_window(game->mlx, game->win, game->collectibles_img, x * T, y * T);
	else if (tile == '0') // empty
		mlx_put_image_to_window(game->mlx, game->win, game->empty_img, x * T, y * T);
	else if (tile == 'A') // tree
		mlx_put_image_to_window(game->mlx, game->win,
								game->tree_sprites[game->tree_frame], x * T, y * T);
	else if (tile == 'H') // house
		mlx_put_image_to_window(game->mlx, game->win, game->house_img, x * T, y * T);
	else if (tile == 'T') // tower
		mlx_put_image_to_window(game->mlx, game->win, game->tower_img, x * T, y * T);
	else if (tile == 'E') // left castle and exit
		mlx_put_image_to_window(game->mlx, game->win, game->exit_img_left, x * T, y * T);
	else if (tile == 'F') // right castle
		mlx_put_image_to_window(game->mlx, game->win, game->exit_img_right, x * T, y * T);
	// Enemies and player are dinamic elements,so they are rendered in dedicated functions
}

//Renders single row
void render_map_row(t_game *game, int y)
{
	int x;

	x = 0;
	while (x <= game->width - 1)
	{
		render_tile(game, game->map[y][x], x, y);
		x++;
	}
}

//Renders entire map
int render_map(t_game *game)
{
	int y;

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
	return (0);
}

//Handle tree animations
void update_tree_animations(t_game *game)
{
	game->tree_timer++;
	if (game->tree_timer >= 30)
	{
		game->tree_frame++;
		if (game->tree_frame >= 6)
			game->tree_frame = 0;
		game->tree_timer = 0;
	}
}