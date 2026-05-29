/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:49:35 by lonulli           #+#    #+#             */
/*   Updated: 2025/05/28 16:49:37 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_H
# define CUBE_H

# include "../mlx/mlx.h"
# include "libft.h"
# include <X11/keysym.h>
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdio.h>
# include <sys/types.h>

# define WIDTH 1920
# define HEIGHT 1080

# define ERR_INVALID_CHAR "Error: Invalid char found\n"
# define ERR_MAP_NOT_CLOSED "Error: Map not closed\n"
# define ERR_MISSING_INFO "Error: Info are missing\n"
# define ERR_NO_VALID_SURR "Error: Player has no valid surroundings\n"
# define ERR_PLAYER_COUNT "Error: Player count is invalid\n"
# define ERR_INVALID_MAP "Error: Invalid map\n"
# define ERR_MALLOC_FAIL "Error: calloc/malloc failed\n"
# define ERR_TOO_MANY_VALUES "Error: a color can only contain 3 values \n"
# define ERR_OUT_OF_RANGE "Error: color value must be in range [0,255]\n"
# define ERR_ONLY_DIGITS "Error: color only contain digits\n"
# define ERR_INV_TEXTURE "Error: texture not found\n"
# define ERR_DOOR_SURROUNDED "Error: door must be sorrounded\n"
# define ERR_MULTIPLE_NL "Error: multiple new lines found\n"

typedef enum e_err
{
	E_INV_CHAR = 1 << 1,
	E_MAP_CLOSED = 1 << 2,
	E_MISS_INFO = 1 << 3,
	E_NO_VAL_SURR = 1 << 4,
	E_PCOUNT = 1 << 5,
	E_INV_MAP = 1 << 6,
	E_MAL_FAIL = 1 << 7,
	E_ERR_VALUES = 1 << 8,
	E_OUT_OF_RANGE = 1 << 9,
	E_ONLY_DIG = 1 << 10,
	E_INV_TEX = 1 << 11,
	E_DOOR = 1 << 12,
	E_MULT_NL = 1 << 13,
}					t_err;

typedef struct s_map
{
	char			**map;
	int				is_map_valid;
	int				map_height;
	char			*no_txt_path;
	char			*so_txt_path;
	char			*we_txt_path;
	char			*ea_txt_path;
	char			*door_txt_path[4];
	char			*ceiling_info;
	char			*floor_info;
	unsigned int	f_rgb;
	unsigned int	c_rgb;
	int				map_width;
	int				map_start;
	int				map_end;
}					t_map;

typedef struct s_player
{
	double			pos_x;
	double			pos_y;
	char			facing_dir;
	int				exists;
	double			curr_pos_x;
	double			curr_pos_y;
	double			dir_x;
	double			dir_y;
	double			curr_frame;
	double			prev_frame;

}					t_player;

typedef struct s_camera
{
	double			plane_x;
	double			plane_y;
}					t_camera;

typedef struct s_texture
{
	void			*img;
	int				width;
	int				height;
	unsigned int	*img_ptr;
	int				bits_per_pixel;
	int				line_lenght;
	int				endian;
}					t_texture;

typedef struct s_mlx_data
{
	void			*mlx_ptr;
	void			*win_ptr;
	void			*img;
	void			*img_ptr;
	void			*img_addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	int				map_ptr;
	t_map			*map;
	t_player		*player;
	t_camera		*camera;
	t_texture		*nord;
	t_texture		*sud;
	t_texture		*est;
	t_texture		*west;
	t_texture		*door[4];
	t_texture		*desk;
	int				err_type;
}					t_data;

typedef struct s_ray
{
	int				x;
	double			hitpoint_x;
	double			hitpoint_y;
	double			ray_y;
	double			ray_x;
	double			dist_x;
	double			dist_y;
	double			side_dist_x;
	double			side_dist_y;
	double			ray_dir_x;
	double			ray_dir_y;
	int				side;
	char			identifier;
	int				x_map;
	int				y_map;
}					t_ray;

typedef struct s_minimap
{
	int				start_x;
	int				start_y;
	int				end_x;
	int				end_y;
}					t_minimap;

// Map related functions

char				**fill_map(t_data *data);
void				check_format(char *arg);
void				is_map_valid(t_data *data);
void				is_map_closed(t_data *data);
int					check_and_set(t_data *data, char c, int x, int y);
int					check_zero_surroundings(t_data *data, char **tab, int x,
						int y);
void				extract_map_only(t_data *data);
void				get_map_infos(t_data *data);
void				are_all_info_present(t_data *data);
void				extract_map_only(t_data *data);
int					is_info(char *curr);
int					skip_white_spaces(char *map);
void				find_start_and_end_of_path(int *start, int *end,
						char *map_line);

// Map utils

int					is_coordinate_valid(char **tab, int x, int y);
void				check_multiple_nl(t_data *data, char **map, int y, int x);
int					are_sorroundings_valid(t_data *data, char c);
int					is_coordinate_valid(char **tab, int x, int y);
int					is_space(char c);
void				print_err_and_free(t_data *data, void *ptr);

// General strs utils

char				**ft_strscat(char **map, char *str);
void				print_strs(char **map);
int					ft_strslen(char **strs);
void				free_strs(char **strs);
int					free_everything(t_data *data);
int					free_exit(t_data *data);

// Ray casting

void				raycasting(t_data *data, t_player *player, t_camera *camera,
						char *identifier);
void				setup_ray_casting(t_data *data, t_player *player,
						t_camera *camera);
void				setup_direction(t_player *player);
int					find_height(int x, int y, t_ray *ray, t_data *data);
t_texture			*choose_texture(t_ray *ray, t_data *data);
unsigned int		get_color(t_ray *ray, t_data *data, int height, int y);
void				add_list(t_list **rays, t_ray *ray, int x, int y);
t_ray				*copy_ray(t_ray *old);
void				draw_sprite(t_list **ray, t_data *data);
char				see_in_front(t_data *data);
int					check_identifier(char map, char *identifier);

// Textures

void				set_texture(t_data *data);
void				free_textures(t_data *data);

// Convert

void				rgb_converter(t_data *data, char x);
void				ft_print_rgb(unsigned int *RGB);

// Game Loop
void				game_loop(t_data *data, t_player *player, t_map *map,
						t_camera *camera);

// Draw
void				draw(t_data *data);
void				draw_mini_map(t_data *data);
void				draw_background(t_data *data);
void				draw_wall(int x, int y, t_ray *ray, t_data *data);
void				fill_square(int x, int y, t_data *data, int color);
void				set_texture(t_data *data);
void				my_mlx_pixel_put(t_data *data, int x, int y,
						unsigned int color);

// Hooks
int					key_hook(int keycode, t_data *data);
int					mouse_hook(int keycode, int x, int y, t_data *data);
int					rotate_player(int keycode, t_data *data, char dir);
void				norm_direction(t_data *data);

#endif
