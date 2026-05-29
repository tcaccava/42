/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_and_error_handling.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:18:55 by lonulli           #+#    #+#             */
/*   Updated: 2025/05/16 10:19:31 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "libft.h"
#include <stdlib.h>

static void	print_err_and_free_map(t_data *data);

void	print_err_and_free(t_data *data, void *ptr)
{
	print_err_and_free_map(data);
	if (data->err_type & E_MAL_FAIL)
		ft_putstr_fd(ERR_MALLOC_FAIL, 2);
	if (data->err_type & E_ERR_VALUES)
		ft_putstr_fd(ERR_TOO_MANY_VALUES, 2);
	if (data->err_type & E_OUT_OF_RANGE)
		ft_putstr_fd(ERR_OUT_OF_RANGE, 2);
	if (data->err_type & E_ONLY_DIG)
		ft_putstr_fd(ERR_ONLY_DIGITS, 2);
	if (data->err_type & E_INV_TEX)
		ft_putstr_fd(ERR_INV_TEXTURE, 2);
	free(ptr);
	free_everything(data);
	exit(EXIT_FAILURE);
}

static void	print_err_and_free_map(t_data *data)
{
	if (data->err_type & E_INV_CHAR)
		ft_putstr_fd(ERR_INVALID_CHAR, 2);
	if (data->err_type & E_MAP_CLOSED)
		ft_putstr_fd(ERR_MAP_NOT_CLOSED, 2);
	if (data->err_type & E_OUT_OF_RANGE)
		ft_putstr_fd(ERR_OUT_OF_RANGE, 2);
	if (data->err_type & E_MISS_INFO)
		ft_putstr_fd(ERR_MISSING_INFO, 2);
	if (data->err_type & E_NO_VAL_SURR)
		ft_putstr_fd(ERR_NO_VALID_SURR, 2);
	if (data->err_type & E_PCOUNT)
		ft_putstr_fd(ERR_PLAYER_COUNT, 2);
	if (data->err_type & E_INV_MAP)
		ft_putstr_fd(ERR_INVALID_MAP, 2);
	if (data->err_type & E_DOOR)
		ft_putstr_fd(ERR_DOOR_SURROUNDED, 2);
	if (data->err_type & E_MULT_NL)
		ft_putstr_fd(ERR_MULTIPLE_NL, 2);
}

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

int	free_everything(t_data *data)
{
	free(data->map->ea_txt_path);
	free(data->map->no_txt_path);
	free(data->map->so_txt_path);
	free(data->map->we_txt_path);
	free(data->map->ceiling_info);
	free(data->map->floor_info);
	free_strs(data->map->map);
	if (data->img)
		mlx_destroy_image(data->mlx_ptr, data->img);
	free_textures(data);
	if (data->win_ptr)
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	if (data->mlx_ptr)
		mlx_destroy_display(data->mlx_ptr);
	free(data->mlx_ptr);
	return (0);
}

int	free_exit(t_data *data)
{
	free_everything(data);
	exit(0);
	return (0);
}
