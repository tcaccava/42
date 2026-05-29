/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:31:30 by girizzi           #+#    #+#             */
/*   Updated: 2025/08/23 19:35:42 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	map_format(char *map_file)
{
	int	len;

	len = ft_strlen(map_file);
	if (map_file == 0)
		return (0);
	if (len < 5)
		return (0);
	if (ft_strncmp(map_file + len - 4, ".ber", 4) != 0)
		return (0);
	return (1);
}

int	validate_args(char **av, int ac)
{
	if (ac != 2)
		error_message("Invalid arguments! Usage: ./so_long <map_name.ber>");
	if (map_format(av[1]) == 0)
		error_message("Invalid map argument! \"<name>.ber\"");
	return (1);
}
