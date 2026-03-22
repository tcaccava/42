/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 04:09:26 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/03 05:42:08 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Global variable to save the map filename,necessary for restart the game so that the program knows which map to load when pressing R
static char *g_map_filename = NULL;

// Handles keypress
int handle_keypress(int key, t_game *game)
{
	int new_x;
	int new_y;

	new_x = game->player_x;
	new_y = game->player_y;

	if (key == KEY_ESC) // pressing ESC exits the game
	{
		ft_printf("HUMAN, HOW DARE YOU CLOSE ME?\n");
		cleanup(game);
		exit(0);
	}

	// Handles inputs in victory or gameover state
	if (game->game_state == GAME_OVER || game->game_state == VICTORY)
	{
		if (key == KEY_R && g_map_filename)
		{
			restart_game(game, g_map_filename);
			return (0);
		}
		return (0); // ignores inputs other than R during victory or gameover state
	}

	// Inputs during normal game
	game->player_state = IDLE; // default player state is idle

	if (key == KEY_W) // up
	{
		new_y--;
		game->player_state = WALK;
	}
	else if (key == KEY_A) // left
	{
		new_x--;
		game->player_dir = DIR_LEFT;
		game->player_state = WALK;
	}
	else if (key == KEY_S) // down
	{
		new_y++;
		game->player_state = WALK;
	}
	else if (key == KEY_D) // right
	{
		new_x++;
		game->player_dir = DIR_RIGHT;
		game->player_state = WALK;
	}

	if (game->player_state == WALK)
		move_player(game, new_x, new_y);

	return (0);
}

// Sets map filename
void set_map_filename(char *filename)
{
	if (g_map_filename)
		free(g_map_filename);
	g_map_filename = ft_strdup(filename);
}

// Returns map filename
char *get_map_filename(void)
{
	return g_map_filename;
}