/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:17:40 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 08:21:04 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_BONUS_H
# define SO_LONG_BONUS_H

# include "mlx.h"
# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/time.h>

# define TILE_SIZE 128
# define ESC_KEY 65307
# define W_KEY 119
# define A_KEY 97
# define S_KEY 115
# define D_KEY 100
# define UP_KEY 65362
# define DO_KEY 65364
# define LO_KEY 65361
# define RO_KEY 65363

// Animation
# define IDLE_FRAMES 5
# define IDLE_ANIMATION_SPEED 200000
# define ENEMY_SPEED 6000 // Lower is faster

typedef struct s_flood_data
{
	int	collectibles;
	int	exits;
}	t_flood_data;

typedef struct s_player_animation
{
	void			*idle_frames[IDLE_FRAMES];
	int				current_frame;
	long			last_frame_time;
}	t_player_animation;

typedef struct s_enemy_animation
{
	void			*n_idle_frames[IDLE_FRAMES];
	int				n_current_frame;
	long			n_last_frame_time;
}	t_enemy_animation;

typedef struct s_images
{
	void				*wall;
	void				*floor;
	void				*player;
	void				*collectible;
	void				*exit;
	void				*enemy;
	void				*game_over;
	t_player_animation	player_anim;
	t_enemy_animation	enemy_anim;
}	t_images;

typedef struct s_enemy
{
	int		x;
	int		y;
	int		direction;
}	t_enemy;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	char		**map;
	int			map_width;
	int			map_height;
	int			player_x;
	int			player_y;
	int			collectibles;
	int			moves;
	int			enemy_count;
	int			is_game_over;
	t_enemy		*enemies;
	t_images	images;
}	t_game;

//so_long
void	error_message(char *message);
int		key_handler(int keycode, t_game *game);

//read_lines
char	*read_single_line(int fd);

//read_map
char	**read_map(char *file);

//validate_args
int		map_format(char *map_file);
int		validate_args(char **av, int ac);

//validate_map
void	validate_map(char **map);

//validate_play
void	validate_playability(char **map);

//count_enemies
int		count_enemies(char **map);
void	find_all_enemies(t_game *game);

//init_game
int		count_collectibles(char **map);
void	init_game_struct(t_game *game, char **map);
void	init_enemies(t_game *game);

//render
void	display_moves(t_game *game);
void	render_map(t_game *game);

//game_loop
int		animation_loop(t_game *game);
void	start_game(t_game *game);
int		close_game(t_game *game);

//game_over
void	game_over(t_game *game);

//animation_control
long	get_current_time_microseconds(void);

//player_move
int		handle_movement(int keycode, t_game *game);

//player_animation
void	init_player_animations(t_game *game);
void	update_player_animation(t_game *game);
void	*get_current_player_sprite(t_game *game);
void	load_idle_frames(t_game *game);

//enemies_animation
void	init_enemy_animations(t_game *game);
void	update_enemy_animation(t_game *game);
void	*get_current_enemy_sprite(t_game *game);
void	load_enemy_frames(t_game *game);

//enemies_move
void	move_enemies_patrol(t_game *game);

//enemies_update
void	update_enemies_on_map(t_game *game);
void	free_enemies(t_game *game);

//free_all
void	free_player_animations(t_game *game);
void	free_images(t_game *game);
void	free_window(t_game *game);
void	free_map(t_game *game);

#endif
