/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:42:51 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:42:52 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "../libft/libft.h"
# include "../mlx/mlx.h"
# include <fcntl.h>
# include <stdlib.h>

# define TILE_SIZE 32

typedef struct s_position
{
	int	x;
	int	y;
}	t_position;

typedef struct s_map
{
	char		**grid;
	int			width;
	int			height;
	int			collectibles;
	int			exits;
	int			players;
	t_position	player_pos;
	t_position	exit_pos;
}	t_map;

typedef struct s_img
{
	void	*img;
	int		width;
	int		height;
}	t_img;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_map		map;
	t_img		wall;
	t_img		floor;
	t_img		player;
	t_img		collect;
	t_img		exit;
	int			moves;
	int			collected;
	int			cannot_collect_all;
	int			invalid_char;
}	t_game;

typedef struct s_flood_data
{
	char	**map;
	int		collectibles;
	int		exit_found;
	t_game	*game;
}	t_flood_data;

/* Main functions */
int		main(int argc, char **argv);

/* Initialization */
int		init_game(t_game *game, char *map_path);
int		init_mlx(t_game *game);
int		load_textures(t_game *game);

/* Map parsing */
int		parse_map(t_game *game, char *map_path);
int		check_file_extension(char *filename);
int		read_map(t_game *game, char *map_path);
void	get_map_dimensions(t_game *game, char *map_content);
char	**create_map_grid(t_game *game, char *map_content);

/* Map validation */
int		validate_map(t_game *game);
int		check_rectangular(t_game *game);
int		check_walls(t_game *game);
int		check_components(t_game *game);

/* Path validation */
int		is_valid_path(t_game *game);
int		flood_fill(char **temp_map, int x, int y, t_position *targets);
char	**duplicate_map(t_game *game);
void	free_temp_map(char **temp_map, int height);
void	mark_collectibles_as_visited(char **map, t_game *game);
int		check_valid_path(t_game *game);
void	flood_fill_collect(t_flood_data *data, int x, int y);
void	flood_fill_exit(t_flood_data *data, int x, int y);
int		can_collect_all_items(t_game *game);
int		can_reach_exit(t_game *game);
/* Game rendering */
void	render_game(t_game *game);
void	render_tile(t_game *game, t_img img, int x, int y);
void	update_window(t_game *game);
void	render_hidden_exit(t_game *game, int x, int y);
/* Input handling */
int		handle_keypress(int keycode, t_game *game);
int		handle_window_close(t_game *game);
void	move_player(t_game *game, int x, int y);
void	find_exit_position(t_game *game);

/* Game logic */
void	check_collectible(t_game *game, int x, int y);
void	check_exit(t_game *game);
void	update_game_state(t_game *game);

/* Cleanup */
void	free_map(t_game *game);
void	free_textures(t_game *game);
void	cleanup_game(t_game *game);
int		exit_game(t_game *game, char *error_msg, int exit_code);

#endif
