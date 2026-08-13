/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:16:56 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/06 01:31:21 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "mlx.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>

# define T 32
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_ESC 65307
# define BUFFER_SIZE 42

typedef struct s_enemy
{
	int		x;
	int		y;
}			t_enemy;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_data;

typedef struct s_game
{
	void	*mlx;
	void	*win;
	char	**map;
	int		width;
	int		height;
	void	*wall_img;
	void	*player_img;
	void	*collectibles_img;
	void	*exit_img;
	void	*empty_img;
	void	*enemy_img;
	int		player_x;
	int		player_y;
	void	*player_sprites[3];
	int		current_frame;
	int		anim_timer;
	int		anim_speed;
	int		collectibles;
	int		moves;
	int		on_exit;
	t_data	img;
	t_enemy	*enemies;
	int		num_enemies;
}			t_game;

void		ft_formatspecifier(char specifier, va_list args, int *length);
int			ft_printf(const char *print, ...);
void		ft_putunsignednbr(unsigned int n, int *length);
void		ft_putstr(char *s, int *length);
void		ft_putpointer(void *ptr, int *length);
void		ft_putnbr(int n, int *length);
void		ft_puthexnbr(size_t n, int *length, int uppercase);
void		ft_putchar(char c, int *length);
char		*get_next_line(int fd);
int			read_join(int fd, char **loot);
char		*extract_line(char *loot);
char		*clear_loot(char *loot);
size_t		ft_strlen(const char *s);
char		*ft_strdup(const char *s);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_strchr(const char *s, int c);
char		*ft_strcpy(char *dest, const char *src);
void		cleanup(t_game *game);
void		parse_map(t_game *game, char *file);
int			render_map(t_game *game);
void		render_tile(t_game *game, char tile, int x, int y);
void		render_map_row(t_game *game, int y);
void		handle_input(int key, t_game *game);
int			handle_keypress(int key, t_game *game);
void		display_moves(t_game *game);
void		update_position(t_game *game, int new_x, int new_y);
void		find_player_position(t_game *game, char *line, int row);
void		new_position(int key, int *x, int *y);
void		count_collectibles(t_game *game);
void		handle_collectible(t_game *game, int new_x, int new_y);
void		int_to_str(int num, char *str);
void		init_game(t_game *game, char *file);
void		check_exit(t_game *game);
int			close_game(t_game *game);
int			close_window(t_game *game);
int			count_map_lines(char *file);
void		find_player(t_game *game);
void		update_player(t_game *game, int new_x, int new_y);
void		allocate_map(t_game *game, char *file);
void		fill_map(t_game *game, char *file);
void		load_images(t_game *game);
void		move_player(t_game *game, int new_x, int new_y);
void		check_walls(t_game *game);
void		remove_newline(char *line);
int			is_valid_move(int x, int y, t_game *game);
int			find_path(int x, int y, t_game *game, char **map_copy);
int			is_path_valid(t_game *game);
void		free_map_copy(char **map_copy, t_game *game);
char		**create_map_copy(t_game *game);
void		load_enemies(t_game *game);
void		check_for_enemy(t_game *game, int new_x, int new_y);
int			update_game(t_game *game);
void		update_animation(t_game *game);
void		check_duplicates(t_game *game);
void		check_map_shape(t_game *game);
void		fill_remaining_rows(t_game *game, int i);
void		count_elements(t_game *game, int *player_count, int *exit_count,
				int *collectible_count);
int			is_valid_map_extension(char *f);
int			is_file_empty(char *filename);
char		*ft_itoa(int n);
void		destroy_image_if_not_null(void *mlx, void *img);
void		check_invalid_chars(t_game *game);

#endif