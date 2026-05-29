/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:17:40 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/27 01:07:07 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "mlx.h"
# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

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

// typedef struct s_map
// {
//     char    **lines;
//     int     height;
//     int     width;
// }   t_map;

typedef struct s_flood_data
{
	int	collectibles;
	int	exits;
}	t_flood_data;

typedef struct s_images
{
	void	*wall;
	void	*floor;
	void	*player;
	void	*collectible;
	void	*exit;
}	t_images;

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
	t_images	images;
}	t_game;

//so_long.c
void	error_message(char *message);
int		key_handler(int keycode, t_game *game);

//read_lines.c
char	*read_single_line(int fd);

//read_map.c
char	**read_map(char *file);

//validate_args.c
int		map_format(char *map_file);
int		validate_args(char **av, int ac);

//validate_map.c
void	validate_map(char **map);

//validate_play.c
void	validate_playability(char **map);

//init_game.c
int		count_collectibles(char **map);
void	init_game_struct(t_game *game, char **map);

//render.c
void	render_map(t_game *game);

//game_loop.c
int		animation_loop(t_game *game);
void	start_game(t_game *game);
int		close_game(t_game *game);

//movement.c
int		handle_movement(int keycode, t_game *game);

#endif
