/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_surroundings_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:20:07 by lonulli           #+#    #+#             */
/*   Updated: 2025/05/16 10:20:09 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

int	are_sorroundings_valid(t_data *data, char c)
{
	if (c && (c == 'P' || c == '0' || c == '1' || c == 'W' || c == 'N'
			|| c == 'S' || c == 'E' || c == 'D'))
		return (1);
	else if (!c || is_space(c) == 1)
	{
		data->err_type = E_MAP_CLOSED;
		print_err_and_free(data, NULL);
	}
	else
	{
		data->err_type = E_INV_CHAR;
		print_err_and_free(data, NULL);
	}
	return (0);
}

int	is_coordinate_valid(char **tab, int x, int y)
{
	if (tab && y >= 0 && x >= 0 && x <= (int)ft_strlen(tab[y]) && tab[y]
		&& tab[y][x])
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == 0x20 || c == 0x0c || c == 0x0a || c == 0x0d || c == 0x09
		|| c == 0x0b)
		return (0x1);
	return (0x0);
}

void	check_multiple_nl(t_data *data, char **map, int x, int y)
{
	static size_t	nl_count;

	if (map[y][x] == '\n')
		nl_count++;
	else
		nl_count = 0;
	if (nl_count == 2)
	{
		data->err_type = E_MULT_NL;
		print_err_and_free(data, NULL);
	}
}
