/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fionni <fionni@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 21:05:18 by fionni            #+#    #+#             */
/*   Updated: 2026/02/05 21:05:18 by fionni           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <mlx.h>
# include "libft.h"

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
	int		players;
	int		exits;
	int		collectibles;
}	t_map;

typedef struct s_imgs
{
	void	*player;
	void	*wall;
	void	*exit;
	void	*collectible;
	void	*floor;
}	t_imgs;

typedef struct s_game
{
	void	*mlx;
	void	*win;
	t_map	map;
	t_imgs	img;

	int		player_x;
	int		player_y;
	int		moves;
}	t_game;

void	sl_error(char *msg);
void	sl_perror(char *msg);
void	sl_free_map(t_map *map);
void	sl_free_game(t_game *g);
int		sl_exit_game(t_game *g);
int		sl_validate_extension(char *file);
int		sl_open_map(char *file);
char	*sl_read_line(int fd);
char	*sl_read_file(int fd);
char	*sl_load_raw_map(char *file);
char	**sl_split_map(char *raw);
int		sl_count_height(char **grid);
int		sl_count_width(char **grid);
void	sl_fill_map_struct(t_map *map, char **grid);
void	sl_parse_map(t_map *map, char *file);
void	sl_check_rectangle(t_map *map);
void	sl_check_walls(t_map *map);
void	sl_update_counts(t_map *map, char c);
void	sl_check_contents(t_map *map);
void	sl_check_map(t_map *map);
void	sl_init_mlx(t_game *g);
void	sl_create_window(t_game *g);
void	sl_destroy_window(t_game *g);
void	sl_setup_hooks(t_game *g);
void	sl_start_loop(t_game *g);
void	*sl_load_xpm(t_game *g, char *path);
void	sl_verify_texture(void *img, char *name);
void	sl_load_all_textures(t_game *g);
void	sl_destroy_texture(t_game *g, void *img);
void	sl_destroy_all_textures(t_game *g);
void	sl_put_tile(t_game *g, char c, int x, int y);
void	sl_render_row(t_game *g, int y);
void	sl_render_background(t_game *g);
void	sl_render_map(t_game *g);
void	sl_refresh_window(t_game *g);
int		sl_close_hook(t_game *g);
int		sl_is_direction_key(int key);
void	sl_handle_direction(t_game *g, int key);
int		sl_key_hook(int key, t_game *g);
void	sl_install_hooks(t_game *g);
void	sl_move_to(t_game *g, int nx, int ny);
void	sl_move_up(t_game *g);
void	sl_move_down(t_game *g);
void	sl_move_left(t_game *g);
void	sl_move_right(t_game *g);
int		sl_is_player(char c);
void	sl_set_player(t_game *g, int x, int y);
int		sl_scan_row_for_player(t_game *g, int y);
void	sl_find_player_position(t_game *g);
void	sl_player_dummy(void);
void	sl_strip_cr(char *line);

#endif
