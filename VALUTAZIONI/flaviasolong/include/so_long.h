/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 23:06:09 by faiello           #+#    #+#             */
/*   Updated: 2025/10/16 23:06:13 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "../minilibx-linux/mlx.h"
# include "ft_printf.h"
# include "get_next_line.h"
# include "libft.h"
# include <fcntl.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

# define TILE_SIZE 128

typedef struct s_player
{
	int			x;
	int			y;
}				t_player;

typedef struct s_map
{
	char		**map;
	int			width;
	int			height;
	int			collectibles;
	int			exits;
	t_player	player;
}				t_map;

typedef struct s_texture
{
	void		*img;
	int			width;
	int			height;
}				t_texture;

typedef struct s_assets
{
	t_texture	wall;
	t_texture	floor;
	t_texture	player;
	t_texture	collectible;
	t_texture	exit_closed;
	t_texture	exit_open;
}				t_assets;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_map		map;
	t_assets	assets;
	int			moves;
	int			collected;
	bool		exit_is_open;
}				t_game;

bool			load_map(const char *path, t_map *map);
bool			validate_map(t_map *map);
bool			check_map_path(t_map *map);
bool			init_game(t_game *game, const char *title);
void			destroy_game(t_game *game);
void			render_game(t_game *game);
int				handle_key(int keycode, t_game *game);
int				handle_close(t_game *game);
void			error_exit(char *message, t_game *game);
void			free_map(t_map *map);

#endif
