/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 03:13:50 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/03 03:17:31 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Key functions,updates all the routines and is called in main loop
int update_game(t_game *game)
{
    int j;
    static int game_over_rendered = 0; // gloabal variable to remember if gameover screens has already been rendered,prevents gameover flickering avoiding show_game_over_screen function to be called 60 times a second
    static int victory_rendered = 0;   // same stuff,but for victory state

    // if game is in gameover or victory state,renders everything one last time
    if (game->game_state == GAME_OVER)
    {
        if (!game_over_rendered)
        {
            update_particles(game);
            render_map(game);
            render_particles(game);
            display_moves(game);
            display_timer(game);
            show_game_over_screen(game);

            game_over_rendered = 1;
        }
        return (0);
    }
    else if (game->game_state == VICTORY)
    {
        if (!victory_rendered)
        {
            update_particles(game);
            render_map(game);
            render_particles(game);
            display_moves(game);
            display_timer(game);
            show_victory_screen(game);
            victory_rendered = 1;
        }
        return (0);
    }

    // resets flags when back in gaming
    game_over_rendered = 0;
    victory_rendered = 0;

    // the game is on
    if (game->game_state == PLAYING)
    {
        update_timer(game);
        update_tree_animations(game);
        update_animation(game);
        update_enemy_animations(game);
        move_enemies(game);
    }

    update_particles(game);
    render_map(game);
    // updates enemies and player position
    if (game->game_state == PLAYING)
    {
        render_enemies(game);
        render_player(game);
    }
    render_particles(game);
    // show UI
    display_moves(game);
    display_timer(game);
    return (0);
}