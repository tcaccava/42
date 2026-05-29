/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lonulli <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:18:21 by lonulli           #+#    #+#             */
/*   Updated: 2025/05/16 10:18:23 by lonulli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube.h"
#include "libft.h"

static void	check_color_validity(t_data *data, char *str, unsigned int *rgb,
				char **tmp_color);
static void	check_overflow(t_data *data, char *str, \
unsigned int *rgb, char **tmp_color);
static void	assign_color(t_data *data, unsigned int *rgb, char flag);

void	rgb_converter(t_data *data, char x)
{
	char			*str;
	unsigned int	*rgb;
	int				j;
	char			**tmp_color;

	str = NULL;
	tmp_color = NULL;
	if (x == 'F')
		str = data->map->floor_info;
	else if (x == 'C')
		str = data->map->ceiling_info;
	rgb = ft_calloc(3, sizeof(unsigned int));
	j = 0;
	if (str)
		tmp_color = ft_split(str, ',');
	while (rgb && tmp_color && tmp_color[j] != NULL)
	{
		check_color_validity(data, tmp_color[j], rgb, tmp_color);
		check_overflow(data, tmp_color[j], rgb, tmp_color);
		rgb[j] = ft_atoi(tmp_color[j]);
		j++;
	}
	free_strs(tmp_color);
	assign_color(data, rgb, x);
}

static void	assign_color(t_data *data, unsigned int *rgb, char flag)
{
	if (rgb && flag == 'F')
		data->map->f_rgb = rgb[0] << 16 | rgb[1] << 8 | rgb[2];
	else if (rgb && flag == 'C')
		data->map->c_rgb = rgb[0] << 16 | rgb[1] << 8 | rgb[2];
	free(rgb);
}

static void	check_overflow(t_data *data, char *str, \
unsigned int *rgb, char **tmp_color)
{
	int	n;
	int	s;

	n = 0;
	s = 1;
	while (*str >= '0' && *str <= '9')
	{
		n = (n * 10) + (*str - '0');
		if ((n * s) > 255 || (n * s) < 0)
		{
			data->err_type = E_OUT_OF_RANGE;
			free_strs(tmp_color);
			print_err_and_free(data, rgb);
		}
		str++;
	}
}

static void	check_color_validity(t_data *data, char *str, unsigned int *rgb,
		char **tmp_color)
{
	int	i;

	if (ft_strslen(tmp_color) > 3)
	{
		data->err_type = E_ERR_VALUES;
		free_strs(tmp_color);
		print_err_and_free(data, rgb);
	}
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			i++;
			continue ;
		}
		if (!ft_isdigit(str[i]))
		{
			data->err_type = E_ONLY_DIG;
			free_strs(tmp_color);
			print_err_and_free(data, rgb);
		}
		i++;
	}
}
