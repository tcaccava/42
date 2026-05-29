/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rodolhop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:46:35 by rodolhop          #+#    #+#             */
/*   Updated: 2025/04/30 16:46:36 by rodolhop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

/* Creates a copy of the game map */
char	**duplicate_map(t_game *game)
{
	char	**temp_map;
	int		i;

	temp_map = (char **)malloc(sizeof(char *) * (game->map.height + 1));
	if (!temp_map)
		return (NULL);
	i = 0;
	while (i < game->map.height)
	{
		temp_map[i] = ft_strdup(game->map.grid[i]);
		if (!temp_map[i])
		{
			free_temp_map(temp_map, i);
			return (NULL);
		}
		i++;
	}
	temp_map[i] = NULL;
	return (temp_map);
}

/* Frees memory allocated for a temporary map */
void	free_temp_map(char **temp_map, int height)
{
	int	i;

	i = 0;
	while (i < height && temp_map[i])
	{
		free(temp_map[i]);
		i++;
	}
	free(temp_map);
}
