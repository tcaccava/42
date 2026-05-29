/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 17:36:11 by valucare          #+#    #+#             */
/*   Updated: 2026/03/07 14:09:33 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	close_handler(t_fractal *f)
{
	mlx_destroy_image(f->mlx_ptr, f->img.img_ptr);
	mlx_destroy_window(f->mlx_ptr, f->win_ptr);
	mlx_destroy_display(f->mlx_ptr);
	free(f->mlx_ptr);
	exit(0);
	return (0);
}

int	handle_key(int keysym, t_fractal *f)
{
	if (keysym == 53 || keysym == 65307)
		close_handler(f);
	else if (keysym == 65361 || keysym == 97)
		f->shift_x -= 0.1 * f->zoom;
	else if (keysym == 65363 || keysym == 100)
		f->shift_x += 0.1 * f->zoom;
	else if (keysym == 65362 || keysym == 119)
		f->shift_y -= 0.1 * f->zoom;
	else if (keysym == 65364 || keysym == 115)
		f->shift_y += 0.1 * f->zoom;
	fractal_render(f);
	return (0);
}

int	handle_mouse(int button, int x, int y, t_fractal *f)
{
	double	mouse_re;
	double	mouse_im;

	mouse_re = map(x, -2, 2, WIDTH) * f->zoom + f->shift_x;
	mouse_im = map(y, 2, -2, HEIGHT) * f->zoom + f->shift_y;
	if (button == 4)
	{
		f->zoom *= 0.9;
		f->shift_x = mouse_re - (mouse_re - f->shift_x) * 0.9;
		f->shift_y = mouse_im - (mouse_im - f->shift_y) * 0.9;
	}
	else if (button == 5)
	{
		f->zoom *= 1.1;
		f->shift_x = mouse_re - (mouse_re - f->shift_x) * 1.1;
		f->shift_y = mouse_im - (mouse_im - f->shift_y) * 1.1;
	}
	fractal_render(f);
	return (0);
}
