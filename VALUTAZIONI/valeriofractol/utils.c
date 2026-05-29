/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 16:55:26 by valucare          #+#    #+#             */
/*   Updated: 2026/03/07 15:35:37 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	handle_error(void)
{
	write(2, "Error: invalid arguments.\n", 27);
	write(2, "Possible options:\n", 18);
	write(2, "  - Mandelbrot       -> ./fractol mandelbrot\n", 46);
	write(2, "  - Julia x y        -> ./fractol julia <real> <imag>\n", 55);
	write(2, "  - Burning Ship     -> ./fractol burning_ship\n", 48);
	exit(1);
}

double	map(double val, double new_min, double new_max, double old_max)
{
	return ((new_max - new_min) * val / old_max + new_min);
}

double	ft_atof(char *s)
{
	long	int_part;
	double	dec_part;
	double	pow;
	int		sign;

	int_part = 0;
	dec_part = 0;
	sign = 1;
	pow = 1;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-')
		sign = -1;
	if (*s == '-' || *s == '+')
		s++;
	while (*s >= '0' && *s <= '9')
		int_part = int_part * 10 + (*s++ - '0');
	if (*s == '.')
		s++;
	while (*s >= '0' && *s <= '9')
	{
		pow /= 10;
		dec_part = dec_part + (*s++ - '0') * pow;
	}
	return ((int_part + dec_part) * sign);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
