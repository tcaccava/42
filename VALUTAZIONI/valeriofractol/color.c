/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 16:02:55 by valucare          #+#    #+#             */
/*   Updated: 2026/03/07 14:21:04 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	get_color(int iter, t_fractal *f)
{
	double	t;
	int		r;
	int		g;
	int		b;

	t = (double)iter / (double)f->iterations;
	r = (int)(8.4 * (1 - t) * t * t * t * 255);
	g = (int)(12.6 * (1 - t) * (1 - t) * t * t * 255);
	b = (int)(4.2 * (1 - t) * (1 - t) * (1 - t) * t * 255);
	return ((r << 16) | (g << 8) | b);
}
