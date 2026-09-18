/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 05:05:32 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/06 03:35:38 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Load xpm images
void load_images(t_game *game)
{
	int img_width;
	int img_height;

	// Walls
	game->wall_img = mlx_xpm_file_to_image(game->mlx, "sprites/wall.xpm", &img_width, &img_height);
	game->wall_top_img = mlx_xpm_file_to_image(game->mlx, "sprites/wall_top.xpm", &img_width, &img_height);
	game->wall_bottom_img = mlx_xpm_file_to_image(game->mlx, "sprites/wall_bottom.xpm", &img_width, &img_height);
	game->wall_left_img = mlx_xpm_file_to_image(game->mlx, "sprites/wall_left.xpm", &img_width, &img_height);
	game->wall_right_img = mlx_xpm_file_to_image(game->mlx, "sprites/wall_right.xpm", &img_width, &img_height);
	game->wall_topleft_img = mlx_xpm_file_to_image(game->mlx, "sprites/wall_topleft.xpm", &img_width, &img_height);
	game->wall_topright_img = mlx_xpm_file_to_image(game->mlx, "sprites/wall_topright.xpm", &img_width, &img_height);
	game->wall_bottomleft_img = mlx_xpm_file_to_image(game->mlx, "sprites/wall_bottomleft.xpm", &img_width, &img_height);
	game->wall_bottomright_img = mlx_xpm_file_to_image(game->mlx, "sprites/wall_bottomright.xpm", &img_width, &img_height);

	// Tree
	game->tree_sprites[0] = mlx_xpm_file_to_image(game->mlx, "sprites/tree1.xpm", &img_width, &img_height);
	game->tree_sprites[1] = mlx_xpm_file_to_image(game->mlx, "sprites/tree2.xpm", &img_width, &img_height);
	game->tree_sprites[2] = mlx_xpm_file_to_image(game->mlx, "sprites/tree3.xpm", &img_width, &img_height);
	game->tree_sprites[3] = mlx_xpm_file_to_image(game->mlx, "sprites/tree4.xpm", &img_width, &img_height);
	game->tree_sprites[4] = mlx_xpm_file_to_image(game->mlx, "sprites/tree5.xpm", &img_width, &img_height);
	game->tree_sprites[5] = mlx_xpm_file_to_image(game->mlx, "sprites/tree6.xpm", &img_width, &img_height);

	game->house_img = mlx_xpm_file_to_image(game->mlx, "sprites/house.xpm", &img_width, &img_height);
	game->tower_img = mlx_xpm_file_to_image(game->mlx, "sprites/tower.xpm", &img_width, &img_height);

	// Enemy left sprites
	game->enemy_left_sprites[0] = mlx_xpm_file_to_image(game->mlx, "sprites/enemy1.xpm", &img_width, &img_height);
	game->enemy_left_sprites[1] = mlx_xpm_file_to_image(game->mlx, "sprites/enemy2.xpm", &img_width, &img_height);
	game->enemy_left_sprites[2] = mlx_xpm_file_to_image(game->mlx, "sprites/enemy3.xpm", &img_width, &img_height);
	game->enemy_left_sprites[3] = mlx_xpm_file_to_image(game->mlx, "sprites/enemy4.xpm", &img_width, &img_height);
	game->enemy_left_sprites[4] = mlx_xpm_file_to_image(game->mlx, "sprites/enemy5.xpm", &img_width, &img_height);
	game->enemy_left_sprites[5] = mlx_xpm_file_to_image(game->mlx, "sprites/enemy6.xpm", &img_width, &img_height);
	game->enemy_left_sprites[6] = mlx_xpm_file_to_image(game->mlx, "sprites/enemy7.xpm", &img_width, &img_height);
	game->enemy_left_sprites[7] = mlx_xpm_file_to_image(game->mlx, "sprites/enemy8.xpm", &img_width, &img_height);

	// Enemy right sprites
	game->enemy_right_sprites[0] = mlx_xpm_file_to_image(game->mlx, "sprites/renemy1.xpm", &img_width, &img_height);
	game->enemy_right_sprites[1] = mlx_xpm_file_to_image(game->mlx, "sprites/renemy2.xpm", &img_width, &img_height);
	game->enemy_right_sprites[2] = mlx_xpm_file_to_image(game->mlx, "sprites/renemy3.xpm", &img_width, &img_height);
	game->enemy_right_sprites[3] = mlx_xpm_file_to_image(game->mlx, "sprites/renemy4.xpm", &img_width, &img_height);
	game->enemy_right_sprites[4] = mlx_xpm_file_to_image(game->mlx, "sprites/renemy5.xpm", &img_width, &img_height);
	game->enemy_right_sprites[5] = mlx_xpm_file_to_image(game->mlx, "sprites/renemy6.xpm", &img_width, &img_height);
	game->enemy_right_sprites[6] = mlx_xpm_file_to_image(game->mlx, "sprites/renemy7.xpm", &img_width, &img_height);
	game->enemy_right_sprites[7] = mlx_xpm_file_to_image(game->mlx, "sprites/renemy8.xpm", &img_width, &img_height);

	// Player left sprites
	game->player_left_sprites[0] = mlx_xpm_file_to_image(game->mlx, "sprites/lidle1.xpm",
														 &img_width, &img_height);
	game->player_left_sprites[1] = mlx_xpm_file_to_image(game->mlx, "sprites/lidle2.xpm",
														 &img_width, &img_height);
	game->player_left_sprites[2] = mlx_xpm_file_to_image(game->mlx, "sprites/lidle3.xpm",
														 &img_width, &img_height);
	game->player_left_sprites[3] = mlx_xpm_file_to_image(game->mlx, "sprites/lidle4.xpm",
														 &img_width, &img_height);
	game->player_left_sprites[4] = mlx_xpm_file_to_image(game->mlx, "sprites/lidle5.xpm",
														 &img_width, &img_height);
	game->player_left_sprites[5] = mlx_xpm_file_to_image(game->mlx, "sprites/lidle6.xpm",
														 &img_width, &img_height);
	game->player_left_sprites[6] = mlx_xpm_file_to_image(game->mlx, "sprites/lidle7.xpm",
														 &img_width, &img_height);
	game->player_left_sprites[7] = mlx_xpm_file_to_image(game->mlx, "sprites/lidle8.xpm",
														 &img_width, &img_height);

	// Player right sprites
	game->player_sprites[0] = mlx_xpm_file_to_image(game->mlx, "sprites/idle1.xpm",
													&img_width, &img_height);
	game->player_sprites[1] = mlx_xpm_file_to_image(game->mlx, "sprites/idle2.xpm",
													&img_width, &img_height);
	game->player_sprites[2] = mlx_xpm_file_to_image(game->mlx, "sprites/idle3.xpm",
													&img_width, &img_height);
	game->player_sprites[3] = mlx_xpm_file_to_image(game->mlx, "sprites/idle4.xpm",
													&img_width, &img_height);
	game->player_sprites[4] = mlx_xpm_file_to_image(game->mlx, "sprites/idle5.xpm",
													&img_width, &img_height);
	game->player_sprites[5] = mlx_xpm_file_to_image(game->mlx, "sprites/idle6.xpm",
													&img_width, &img_height);
	game->player_sprites[6] = mlx_xpm_file_to_image(game->mlx, "sprites/idle7.xpm",
													&img_width, &img_height);
	game->player_sprites[7] = mlx_xpm_file_to_image(game->mlx, "sprites/idle8.xpm",
													&img_width, &img_height);

	game->collectibles_img = mlx_xpm_file_to_image(game->mlx,
												   "sprites/collectibles.xpm", &img_width, &img_height);
	game->empty_img = mlx_xpm_file_to_image(game->mlx, "sprites/empty.xpm", &img_width,
											&img_height);
	// Enemy castle
	game->exit_img_left = mlx_xpm_file_to_image(game->mlx, "sprites/castle.xpm", &img_width, &img_height);
	game->exit_img_right = mlx_xpm_file_to_image(game->mlx, "sprites/castle2.xpm", &img_width, &img_height);
}

// Init game systems
void init_game(t_game *game, char *file)
{
	game->mlx = mlx_init();
	game->map = NULL;
	allocate_map(game, file);
	fill_map(game, file);
	load_images(game);
	game->win = mlx_new_window(game->mlx, game->width * T, game->height * T,
							   "so_long");
	// Map validations
	check_invalid_chars(game);
	check_map_shape(game);
	check_walls(game);
	check_duplicates(game);
	if (!is_path_valid(game))
	{
		ft_printf("Error: No valid path found in the map.\n");
		cleanup(game);
		exit(1);
	}
	// Init animations and status
	game->current_frame_right = 0;
	game->current_frame_left = 0;
	game->anim_timer = 0;
	game->anim_speed = 5;
	game->player_dir = DIR_RIGHT;
	game->player_state = IDLE;
	game->tree_frame = 0;
	game->tree_timer = 0;
	game->moves = 0;

	// Inits new system
	game->game_state = PLAYING;
	game->score = 0;
	init_timer(game);
	init_particles(game);
	// Load enemies
	load_enemies(game);

	// Save map filename for restart
	set_map_filename(file);
}

// Moves counter
void display_moves(t_game *game)
{
	char *move_count;
	int x;
	int y;
	int color;

	x = 10;
	y = 10;
	move_count = ft_itoa(game->moves);
	color = 0xFF0000;
	// Pixel shadowing: in MLX we can't set character size,so i shift the move count of 1 pixel to make it appear greater and cleaner
	mlx_string_put(game->mlx, game->win, x, y, color, "Moves: ");
	mlx_string_put(game->mlx, game->win, x + 1, y, color, "Moves: ");
	mlx_string_put(game->mlx, game->win, x, y + 1, color, "Moves: ");
	mlx_string_put(game->mlx, game->win, x + 1, y + 1, color, "Moves: ");
	mlx_string_put(game->mlx, game->win, x + 60, y, color, move_count);
	mlx_string_put(game->mlx, game->win, x + 61, y, color, move_count);
	mlx_string_put(game->mlx, game->win, x + 60, y + 1, color, move_count);
	mlx_string_put(game->mlx, game->win, x + 61, y + 1, color, move_count);
	free(move_count);
}

// Remove /n
void remove_newline(char *line)
{
	int len;

	len = ft_strlen(line);
	if (line[len - 1] == '\n')
	{
		line[len - 1] = '\0';
	}
}

void ft_putunsignednbr(unsigned int n, int *length)
{
	if (n >= 10)
		ft_putunsignednbr(n / 10, length);
	ft_putchar('0' + (n % 10), length);
}

