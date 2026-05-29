/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 19:34:08 by valucare          #+#    #+#             */
/*   Updated: 2026/03/07 15:12:41 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	put_pixel(int x, int y, int color, t_img *img)
{
	int	offset;

	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->pixels_ptr + offset) = color;
}

static int	iterate(int type, t_complex z, t_complex c, int max_iter)
{
	int	i;

	i = 0;
	while (i < max_iter)
	{
		if (type == BURNING_SHIP)
		{
			z.re = fabs(z.re);
			z.im = fabs(z.im);
		}
		z = sum_complex(square_complex(z), c);
		if ((z.re * z.re) + (z.im * z.im) > 4.0)
			return (i);
		i++;
	}
	return (max_iter);
}

static void	handle_pixel(int x, int y, t_fractal *f)
{
	t_complex	z;
	t_complex	c;
	int			iter;

	z.re = map(x, -2.0, 2.0, WIDTH) * f->zoom + f->shift_x;
	z.im = map(y, 2.0, -2.0, HEIGHT) * f->zoom + f->shift_y;
	if (f->type == MANDELBROT || f->type == BURNING_SHIP)
		c = z;
	else
	{
		c.re = f->julia_re;
		c.im = f->julia_im;
	}
	iter = iterate(f->type, z, c, f->iterations);
	if (iter == f->iterations)
		put_pixel(x, y, 0x000000, &f->img);
	else
		put_pixel(x, y, get_color(iter, f), &f->img);
}

void	fractal_render(t_fractal *f)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			handle_pixel(x, y, f);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(f->mlx_ptr, f->win_ptr, f->img.img_ptr, 0, 0);
}
