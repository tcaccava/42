/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 18:34:22 by valucare          #+#    #+#             */
/*   Updated: 2026/03/07 14:09:16 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	fractal_defaults(t_fractal *f)
{
	f->zoom = 1.0;
	f->shift_x = 0.0;
	f->shift_y = 0.0;
	f->iterations = 100;
}

void	fractal_init(t_fractal *f)
{
	f->mlx_ptr = mlx_init();
	if (!f->mlx_ptr)
		handle_error();
	f->win_ptr = mlx_new_window(f->mlx_ptr, WIDTH, HEIGHT, "fract'ol");
	if (!f->win_ptr)
	{
		free(f->mlx_ptr);
		handle_error();
	}
	f->img.img_ptr = mlx_new_image(f->mlx_ptr, WIDTH, HEIGHT);
	if (!f->img.img_ptr)
	{
		mlx_destroy_window(f->mlx_ptr, f->win_ptr);
		free(f->mlx_ptr);
		handle_error();
	}
	f->img.pixels_ptr = mlx_get_data_addr(f->img.img_ptr, &f->img.bpp,
			&f->img.line_len, &f->img.endian);
	fractal_defaults(f);
	mlx_key_hook(f->win_ptr, handle_key, f);
	mlx_mouse_hook(f->win_ptr, handle_mouse, f);
	mlx_hook(f->win_ptr, 17, 0, close_handler, f);
}
