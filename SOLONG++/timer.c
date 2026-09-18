/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer_score.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:00:00 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/06 10:00:00 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

//Init timer
void init_timer(t_game *game)
{
    gettimeofday(&game->start_time, NULL);//gettimeofday fills a timeval struct with seconds and us passed since january 1st 1970(epoch time)
    game->game_time_seconds = 0;
}

//Update timer
void update_timer(t_game *game)
{
    struct timeval current_time;
    long elapsed_usec;
    
    if (game->game_state != PLAYING)
        return;
    
    gettimeofday(&current_time, NULL);//gtets current time
    elapsed_usec = (current_time.tv_sec - game->start_time.tv_sec) * 1000000 +
                   (current_time.tv_usec - game->start_time.tv_usec);// elapsed time in microsecond between start time and current time
    
    game->game_time_seconds = elapsed_usec / 1000000;
}


void display_timer(t_game *game)
{
    char *time_str;
    int minutes;
    int seconds;
    char time_display[20];
    int x = 10;
    int y = 30;
    int color = 0xFF0000;//red
    
    minutes = game->game_time_seconds / 60;
    seconds = game->game_time_seconds % 60;
    
    // MM:SS format
    time_display[0] = (minutes / 10) + '0';
    time_display[1] = (minutes % 10) + '0';
    time_display[2] = ':';
    time_display[3] = (seconds / 10) + '0';
    time_display[4] = (seconds % 10) + '0';
    time_display[5] = '\0';
    
    // Pixel shifting
    mlx_string_put(game->mlx, game->win, x, y, color, "Time: ");
    mlx_string_put(game->mlx, game->win, x + 1, y, color, "Time: ");
    mlx_string_put(game->mlx, game->win, x, y + 1, color, "Time: ");
    mlx_string_put(game->mlx, game->win, x + 1, y + 1, color, "Time: ");
    
    mlx_string_put(game->mlx, game->win, x + 50, y, color, time_display);
    mlx_string_put(game->mlx, game->win, x + 51, y, color, time_display);
    mlx_string_put(game->mlx, game->win, x + 50, y + 1, color, time_display);
    mlx_string_put(game->mlx, game->win, x + 51, y + 1, color, time_display);
}