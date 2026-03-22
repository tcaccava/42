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
#define SO_LONG_H

#include "mlx.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

#define T 64 // tile size
// keycodes
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_R 114
#define KEY_ESC 65307
#define BUFFER_SIZE 42
#define MAX_PARTICLES 500 // particles number

typedef enum e_dir
{
	DIR_LEFT,
	DIR_RIGHT
} t_dir;

typedef enum e_game_state
{
	PLAYING,
	GAME_OVER,
	VICTORY
} t_game_state;

typedef struct s_particle
{
	float x;
	float y;
	float vel_x;
	float vel_y;
	int life;
	int max_life;
	int color;
	int active;
} t_particle;

typedef struct s_position
{
    int x;
    int y;
} t_position;

typedef struct s_enemy
{
	int x;
	int y;
	int current_frame;
	int anim_timer;
	t_dir dir;
	int move_timer;
	int is_chasing;
	int chase_timer;

} t_enemy;

typedef enum e_player_state
{
	IDLE,
	WALK
} t_player_state;

typedef struct s_game
{
	void *mlx;
	void *win;
	char **map;
	int width;
	int height;
	void *wall_img;
	void *player_img;
	void *collectibles_img;
	void *exit_img;
	void *empty_img;
	void *enemy_left_sprites[8];
	void *enemy_right_sprites[8];
	void *wall_top_img;
	void *wall_bottom_img;
	void *wall_left_img;
	void *wall_right_img;
	void *wall_topleft_img;
	void *wall_topright_img;
	void *wall_bottomleft_img;
	void *wall_bottomright_img;
	void *tree_sprites[6];
	int tree_frame;
	int tree_timer;
	void *house_img;
	void *tower_img;
	void *exit_img_left;
	void *exit_img_right;

	int player_x;
	int player_y;
	t_dir player_dir;
	t_player_state player_state;
	void *player_sprites[8];
	void *player_left_sprites[8];
	int current_frame_right;
	int current_frame_left;
	int anim_timer;
	int anim_speed;

	int collectibles;
	int moves;
	int on_exit;
	t_game_state game_state;

	struct timeval start_time;
	int game_time_seconds;
	int score;
	t_particle particles[MAX_PARTICLES];

	t_enemy *enemies;
	int num_enemies;
} t_game;

void ft_formatspecifier(char specifier, va_list args, int *length);
int ft_printf(const char *print, ...);
void ft_putunsignednbr(unsigned int n, int *length);
void ft_putstr(char *s, int *length);
void ft_putpointer(void *ptr, int *length);
void ft_putnbr(int n, int *length);
void ft_puthexnbr(size_t n, int *length, int uppercase);
void ft_putchar(char c, int *length);
char *get_next_line(int fd);
int read_join(int fd, char **loot);
char *extract_line(char *loot);
char *clear_loot(char *loot);
size_t ft_strlen(const char *s);
char *ft_strdup(const char *s);
char *ft_strjoin(char *s1, char *s2);
char *ft_strchr(const char *s, int c);
char *ft_strcpy(char *dest, const char *src);
void cleanup(t_game *game);
void parse_map(t_game *game, char *file);
int render_map(t_game *game);
void render_tile(t_game *game, char tile, int x, int y);
void render_map_row(t_game *game, int y);
void handle_input(int key, t_game *game);
int handle_keypress(int key, t_game *game);
void display_moves(t_game *game);
void update_position(t_game *game, int new_x, int new_y);
void find_player_position(t_game *game, char *line, int row);
void new_position(int key, int *x, int *y);
void count_collectibles(t_game *game);
void handle_collectible(t_game *game, int new_x, int new_y);
void int_to_str(int num, char *str);
void init_game(t_game *game, char *file);
void check_exit(t_game *game);
int close_window(t_game *game);
int count_map_lines(char *file);
void find_player(t_game *game);
void update_player(t_game *game, int new_x, int new_y);
void allocate_map(t_game *game, char *file);
void fill_map(t_game *game, char *file);
void load_images(t_game *game);
void move_player(t_game *game, int new_x, int new_y);
void check_walls(t_game *game);
void remove_newline(char *line);
int is_valid_move(int x, int y, t_game *game);
int find_path(int x, int y, t_game *game, char **map_copy);
int is_path_valid(t_game *game);
void free_map_copy(char **map_copy, t_game *game);
char **create_map_copy(t_game *game);
void load_enemies(t_game *game);
void check_for_enemy(t_game *game, int new_x, int new_y);
int update_game(t_game *game);
void update_animation(t_game *game);
void check_duplicates(t_game *game);
void check_map_shape(t_game *game);
void fill_remaining_rows(t_game *game, int i);
void count_elements(t_game *game, int *player_count, int *exit_count,
					int *collectible_count);
int is_valid_map_extension(char *f);
int is_file_empty(char *filename);
char *ft_itoa(int n);
void destroy_image_if_not_null(void *mlx, void *img);
void check_invalid_chars(t_game *game);
void update_enemy_animations(t_game *game);
void render_enemies(t_game *game);
int count_enemies(t_game *game);
void render_player(t_game *game);
void move_enemies(t_game *game);
int is_position_occupied_by_enemy(t_game *game, int x, int y, int current_enemy_index);
int check_player_enemy_collision(t_game *game);
void init_timer(t_game *game);
void update_timer(t_game *game);
void display_timer(t_game *game);
void calculate_score(t_game *game);
void show_game_over_screen(t_game *game);
void show_victory_screen(t_game *game);
void restart_game(t_game *game, char *filename);
void init_particles(t_game *game);
void spawn_particles(t_game *game, int x, int y);
void update_particles(t_game *game);
void render_particles(t_game *game);
void draw_rectangle(t_game *game, int x, int y, int width, int height, int color);
void draw_text_centered(t_game *game, char *text, int y, int color);
void set_map_filename(char *filename);
void update_tree_animations(t_game *game);
int calculate_distance(int x1, int y1, int x2, int y2);
int should_start_chasing(t_game *game, int enemy_index);
int should_stop_chasing(t_game *game, int enemy_index);
t_position calculate_chase_move(t_game *game, int enemy_index);


#endif