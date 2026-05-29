/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strs_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:19:45 by lonulli           #+#    #+#             */
/*   Updated: 2025/05/16 10:19:46 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"

char	**ft_strscat(char **map, char *str)
{
	int		i;
	int		strs_num;
	char	**new_array;

	i = 0;
	strs_num = ft_strslen(map);
	new_array = ft_calloc(strs_num + 2, sizeof(char *));
	if (!new_array)
	{
		ft_putstr_fd("Malloc error\n", 2);
		return (NULL);
	}
	while (map && map[i])
	{
		new_array[i] = map[i];
		i++;
	}
	new_array[i] = str;
	free(map);
	return (new_array);
}

int	ft_strslen(char **strs)
{
	int	i;

	i = 0;
	while (strs && strs[i])
		i++;
	return (i);
}

void	print_strs(char **map)
{
	int	i;

	i = 0;
	while (map && map[i])
	{
		printf("%s", map[i]);
		i++;
	}
}
