/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   particles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:30:00 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/06 10:30:00 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Init particles settings
void init_particles(t_game *game)
{
    int i;

    i = 0;
    while (i < MAX_PARTICLES) // 500 particles per collectible
    {
        game->particles[i].active = 0;
        game->particles[i].life = 0;
        game->particles[i].max_life = 0;
        game->particles[i].x = 0;
        game->particles[i].y = 0;
        game->particles[i].vel_x = 0;
        game->particles[i].vel_y = 0;
        game->particles[i].color = 0;
        i++;
    }
}

// Particles spawn system
void spawn_particles(t_game *game, int tile_x, int tile_y)
{
    int i;
    int count;
    int center_x;
    int center_y;

    // spawn point is the center of the tile
    center_x = tile_x * T + T / 2;
    center_y = tile_y * T + T / 2;

    count = 0;
    i = 0;

    while (i < MAX_PARTICLES && count < 500)
    {
        if (!game->particles[i].active)
        {
            game->particles[i].active = 1;

            // initial spawn point is the center of tile with a little random variation of -8 / 8 pixels
            game->particles[i].x = center_x + (rand() % 16 - 8);
            game->particles[i].y = center_y + (rand() % 16 - 8);

            // particles speed is random to simulate the explosion
            game->particles[i].vel_x = (float)(rand() % 12 - 6) * 0.7f;
            game->particles[i].vel_y = (float)(rand() % 12 - 6) * 0.7f;

            // particles life is 75frame + 0 to 40 frames
            game->particles[i].max_life = 75 + (rand() % 40);
            game->particles[i].life = game->particles[i].max_life;

            // ten random colors
            int color_choice = rand() % 10;
            switch (color_choice)
            {
            case 0:
                game->particles[i].color = 0xFFD700;
                break; // gold
            case 1:
                game->particles[i].color = 0xFFA500;
                break; // orange
            case 2:
                game->particles[i].color = 0xFF0000;
                break; // tomato
            case 3:
                game->particles[i].color = 0xFF1493;
                break; // pink
            case 4:
                game->particles[i].color = 0x00FFFF;
                break; // cyan
            case 5:
                game->particles[i].color = 0x00FF00;
                break; // lime Green
            case 6:
                game->particles[i].color = 0x0000FF;
                break; // blu
            case 7:
                game->particles[i].color = 0x9370DB;
                break; // purple
            case 8:
                game->particles[i].color = 0xFFFFFF;
                break; // white
            case 9:
                game->particles[i].color = 0xFFFF00;
                break; // yellow
            }

            count++;
        }
        i++;
    }
}

// Handle particles life
void update_particles(t_game *game)
{
    int i;

    i = 0;
    while (i < MAX_PARTICLES)
    {
        if (game->particles[i].active)
        {
            // updates position with speed
            game->particles[i].x += game->particles[i].vel_x;
            game->particles[i].y += game->particles[i].vel_y;

            // simulate a small gravity effect on the vertical position
            game->particles[i].vel_y += 0.05f;

            // simulate air friction,speed reduces of 1% for each function call
            game->particles[i].vel_x *= 0.99f;
            game->particles[i].vel_y *= 0.99f;

            game->particles[i].life--;

            // dead particle
            if (game->particles[i].life <= 0)
                game->particles[i].active = 0;
        }
        i++;
    }
}

// Renders particles
void render_particles(t_game *game)
{
    int i;
    int px, py;
    int size;
    int j, k;

    i = 0;
    while (i < MAX_PARTICLES)
    {
        if (game->particles[i].active)
        {
            px = (int)game->particles[i].x;
            py = (int)game->particles[i].y;

            int current_color = game->particles[i].color;

            // particles have diamond shape
            mlx_pixel_put(game->mlx, game->win, px + 1, py, current_color);
            mlx_pixel_put(game->mlx, game->win, px, py + 1, current_color);
            mlx_pixel_put(game->mlx, game->win, px + 2, py + 1, current_color);
            mlx_pixel_put(game->mlx, game->win, px + 1, py + 2, current_color);
            mlx_pixel_put(game->mlx, game->win, px + 1, py + 1, current_color);
        }
        i++;
    }
}