/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 04:04:07 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/05 23:05:16 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// Counts enemies
int count_enemies(t_game *game)
{
    int y;
    int x;
    int count;

    count = 0;
    y = 0;
    while (y < game->height)
    {
        x = 0;
        while (x < game->width)
        {
            if (game->map[y][x] == 'N')
                count++;
            x++;
        }
        y++;
    }
    return count;
}

// Mallocs and inits enemy's array
void load_enemies(t_game *game)
{
    int x, y;
    int enemy_count;

    game->num_enemies = count_enemies(game);
    game->enemies = malloc(sizeof(t_enemy) * game->num_enemies);
    if (!game->enemies)
        exit(1);

    enemy_count = 0;
    y = 0;
    while (y < game->height)
    {
        x = 0;
        while (x < game->width)
        {
            if (game->map[y][x] == 'N')
            {
                game->enemies[enemy_count].x = x;
                game->enemies[enemy_count].y = y;
                game->enemies[enemy_count].current_frame = 0;
                game->enemies[enemy_count].anim_timer = 0;
                game->enemies[enemy_count].move_timer = 0;
                game->enemies[enemy_count].chase_timer = 0;
                game->enemies[enemy_count].is_chasing = 0;
                game->enemies[enemy_count].dir = (rand() % 2 == 0) ? DIR_LEFT : DIR_RIGHT; // random left-right spawn
                enemy_count++;
                game->map[y][x] = '0'; // updates the spawn position with empty sprites to prevent ghost enemies
            }
            x++;
        }
        y++;
    }
}

// Handles player-enemies collisions
void check_for_enemy(t_game *game, int new_x, int new_y)
{
    int i;

    i = 0;
    while (i < game->num_enemies)
    {
        if (game->enemies[i].x == new_x && game->enemies[i].y == new_y)
        {
            ft_printf("Game Over! You hit an enemy.\n");
            game->game_state = GAME_OVER;
            return;
        }
        i++;
    }
}

// Update enemy's animation timer
void update_enemy_animations(t_game *game)
{
    int j;

    j = 0;
    while (j < game->num_enemies)
    {
        game->enemies[j].anim_timer++;
        if (game->enemies[j].anim_timer >= 20)
        {
            game->enemies[j].current_frame++;
            if (game->enemies[j].current_frame >= 8)
                game->enemies[j].current_frame = 0;
            game->enemies[j].anim_timer = 0;
        }
        j++;
    }
}

// Render enemy array
void render_enemies(t_game *game)
{
    int j = 0;
    while (j < game->num_enemies)
    {
        void *frame;
        if (game->enemies[j].dir == DIR_LEFT)
            frame = game->enemy_left_sprites[game->enemies[j].current_frame];
        else
            frame = game->enemy_right_sprites[game->enemies[j].current_frame];

        mlx_put_image_to_window(game->mlx, game->win,
                                frame,
                                game->enemies[j].x * T,
                                game->enemies[j].y * T);
        j++;
    }
}

// Handles enemies movement
void move_enemies(t_game *game)
{
    int i;
    int dir;
    int move_speed;

    if (game->game_state != PLAYING) // doesn't updates enemy position if the game is in victory or gameover state
        return;

    i = 0;
    while (i < game->num_enemies)
    {
        // starts chasing
        if (!game->enemies[i].is_chasing && should_start_chasing(game, i))
        {
            game->enemies[i].is_chasing = 1;
            game->enemies[i].chase_timer = 0;
        } // stops chasing
        else if (game->enemies[i].is_chasing && should_stop_chasing(game, i))
        {
            game->enemies[i].is_chasing = 0;
            game->enemies[i].move_timer = 0;
        }

        // determines enemy speed
        if (game->enemies[i].is_chasing)
        {
            move_speed = 150; // faster in chasing
            game->enemies[i].chase_timer++;
        }
        else
        {
            move_speed = 120; // normal speed in random movements
            game->enemies[i].move_timer++;
        }
        // is it time to move?
        int should_move = 0;
        if (game->enemies[i].is_chasing && game->enemies[i].chase_timer >= move_speed)
        {
            should_move = 1;
            game->enemies[i].chase_timer = 0;
        }
        else if (!game->enemies[i].is_chasing && game->enemies[i].move_timer >= move_speed)
        {
            should_move = 1;
            game->enemies[i].move_timer = 0;
        }
        if (should_move)
        {
            t_position new_pos;

            // calculates chase moves
            if (game->enemies[i].is_chasing)
            {
                new_pos = calculate_chase_move(game, i);
            }
            else // random move
            {
                new_pos.x = game->enemies[i].x;
                new_pos.y = game->enemies[i].y;
                dir = rand() % 4; // the new direction is random
                if (dir == 0)     // up
                    new_pos.y--;
                else if (dir == 1) // down
                    new_pos.y++;
                else if (dir == 2) // left
                {
                    new_pos.x--;
                    game->enemies[i].dir = DIR_LEFT;
                }
                else if (dir == 3) // right
                {
                    new_pos.x++;
                    game->enemies[i].dir = DIR_RIGHT;
                }
            }
            // avoids collisions with static elements and prevents enemies overlaps
            if (game->map[new_pos.y][new_pos.x] != '1' &&
                game->map[new_pos.y][new_pos.x] != 'H' &&
                game->map[new_pos.y][new_pos.x] != 'A' &&
                game->map[new_pos.y][new_pos.x] != 'C' &&
                game->map[new_pos.y][new_pos.x] != 'T' &&
                game->map[new_pos.y][new_pos.x] != 'E' &&
                game->map[new_pos.y][new_pos.x] != 'F' &&
                !is_position_occupied_by_enemy(game, new_pos.x, new_pos.y, i)) // prevents enemies overlaps
            {
                // updates positions
                game->enemies[i].x = new_pos.x;
                game->enemies[i].y = new_pos.y;

                if (check_player_enemy_collision(game)) // checks enemy-playes collisions after the movement
                {
                    ft_printf("Game Over! Enemy hit you\n");
                    game->game_state = GAME_OVER;
                    return;
                }
            }
        }
        i++;
    }
}

// Avoids enemies overlap
int is_position_occupied_by_enemy(t_game *game, int x, int y, int current_enemy_index)
{
    int i;

    i = 0;
    while (i < game->num_enemies)
    {
        if (i != current_enemy_index)
        {
            if (game->enemies[i].x == x && game->enemies[i].y == y)
                return (1); // occupied position
        }
        i++;
    }
    return (0); // free position
}

// Checks player-enemy collisions
int check_player_enemy_collision(t_game *game)
{
    int i;

    i = 0;
    while (i < game->num_enemies)
    {
        if (game->enemies[i].x == game->player_x &&
            game->enemies[i].y == game->player_y)
        {
            return (1); // collision
        }
        i++;
    }
    return (0); // no collision
}

// Calculates the Manhattan distance
int calculate_distance(int x1, int y1, int x2, int y2)
{
    int dx = x1 - x2;
    int dy = y1 - y2;

    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;

    return dx + dy;
}

// Should enemy chase you?
int should_start_chasing(t_game *game, int enemy_index)
{
    int distance = calculate_distance(
        game->enemies[enemy_index].x,
        game->enemies[enemy_index].y,
        game->player_x,
        game->player_y);

    return distance <= 3; // starts chasing over 3 tiles
}

// Should enemy stop chasing you?
int should_stop_chasing(t_game *game, int enemy_index)
{
    int distance = calculate_distance(
        game->enemies[enemy_index].x,
        game->enemies[enemy_index].y,
        game->player_x,
        game->player_y);

    return distance > 5; // stops chasing over 5 tiles
}

// Where should the enemy move to chase the gamer?
t_position calculate_chase_move(t_game *game, int enemy_index)
{
    t_position new_pos;
    int enemy_x = game->enemies[enemy_index].x;
    int enemy_y = game->enemies[enemy_index].y;
    int player_x = game->player_x;
    int player_y = game->player_y;

    new_pos.x = enemy_x;
    new_pos.y = enemy_y;

    // priority to horizontal move
    if (player_x > enemy_x)
    {
        new_pos.x = enemy_x + 1;
        game->enemies[enemy_index].dir = DIR_RIGHT;
    }
    else if (player_x < enemy_x)
    {
        new_pos.x = enemy_x - 1;
        game->enemies[enemy_index].dir = DIR_LEFT;
    }
    // when horizzontally aligned,move vertically
    else if (player_y > enemy_y)
    {
        new_pos.y = enemy_y + 1;
    }
    else if (player_y < enemy_y)
    {
        new_pos.y = enemy_y - 1;
    }
    // Fix for avoiding obstacles it they are the next chase move
    if (game->map[new_pos.y][new_pos.x] == '1' ||
        game->map[new_pos.y][new_pos.x] == 'A' ||
        game->map[new_pos.y][new_pos.x] == 'H' ||
        game->map[new_pos.y][new_pos.x] == 'T' || 
        game->map[new_pos.y][new_pos.x] == 'C')
    {
        // If horizzontally blocked,try vertically
        if (new_pos.x != enemy_x)
        {
            new_pos.x = enemy_x; // reset x
            if (player_y > enemy_y)
                new_pos.y = enemy_y + 1;
            else if (player_y < enemy_y)
                new_pos.y = enemy_y - 1;
        }
        // If vertically blocked,try horizzontally
        else if (new_pos.y != enemy_y)
        {
            new_pos.y = enemy_y; // reset y
            if (player_x > enemy_x)
            {
                new_pos.x = enemy_x + 1;
                game->enemies[enemy_index].dir = DIR_RIGHT;
            }
            else if (player_x < enemy_x)
            {
                new_pos.x = enemy_x - 1;
                game->enemies[enemy_index].dir = DIR_LEFT;
            }
        }
    }
    return new_pos;
}