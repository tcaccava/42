/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 05:28:31 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/03 07:43:08 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	render_map_row(t_game *game, int y)
{
	int	x;

	x = 0;
	while (x <= game->width - 1)
	{
		render_tile(game, game->map[y][x], x, y);
		x++;
	}
}

void	fill_remaining_rows(t_game *game, int i)
{
	while (i < game->height)
	{
		game->map[i] = NULL;
		i++;
	}
}

void	ft_putunsignednbr(unsigned int n, int *length)
{
	if (n >= 10)
		ft_putunsignednbr(n / 10, length);
	ft_putchar('0' + (n % 10), length);
}

int	is_file_empty(char *filename)
{
	int		fd;
	char	buffer;
	ssize_t	bytes_read;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_printf("Error: Unable to open file %s.\n", filename);
		return (1);
	}
	bytes_read = read(fd, &buffer, 1);
	close(fd);
	if (bytes_read == 0)
	{
		ft_printf("Error: The file %s is empty.\n", filename);
		return (1);
	}
	return (0);
}
