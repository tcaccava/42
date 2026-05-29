/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 17:01:22 by valucare          #+#    #+#             */
/*   Updated: 2026/03/07 15:12:50 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static int	is_valid_double(char *str)
{
	int	i;
	int	dot;

	i = 0;
	dot = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] == '.')
		{
			if (dot)
				return (0);
			dot = 1;
		}
		else if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	validate_args(int ac, char **av, t_fractal *f)
{
	if (ac == 2 && !ft_strncmp(av[1], "mandelbrot", 10))
		f->type = MANDELBROT;
	else if (ac == 4 && !ft_strncmp(av[1], "julia", 5))
	{
		if (!is_valid_double(av[2]) || !is_valid_double(av[3]))
			handle_error();
		f->type = JULIA;
		f->julia_re = ft_atof(av[2]);
		f->julia_im = ft_atof(av[3]);
	}
	else if (ac == 2 && !ft_strncmp(av[1], "burning_ship", 12))
		f->type = BURNING_SHIP;
	else
		handle_error();
}
