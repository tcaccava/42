/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:36:09 by valucare          #+#    #+#             */
/*   Updated: 2026/03/07 14:42:04 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include "mlx.h"

# define WIDTH 800
# define HEIGHT 800

# define MANDELBROT 1
# define JULIA 2
# define BURNING_SHIP 3

typedef struct s_complex
{
	double	re;
	double	im;
}	t_complex;

typedef struct s_img
{
	void	*img_ptr;
	char	*pixels_ptr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_img;

typedef struct s_fractal
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		img;
	int			type;
	double		julia_re;
	double		julia_im;
	double		zoom;
	double		shift_x;
	double		shift_y;
	int			iterations;
}	t_fractal;

void		fractal_init(t_fractal *f);
void		fractal_render(t_fractal *f);
int			handle_mouse(int button, int x, int y, t_fractal *f);
int			handle_key(int keysym, t_fractal *f);
int			close_handler(t_fractal *f);
int			get_color(int iter, t_fractal *f);
t_complex	sum_complex(t_complex c1, t_complex c2);
t_complex	square_complex(t_complex c);
double		map(double val, double new_min, double new_max, double old_max);
void		validate_args(int ac, char **av, t_fractal *f);
double		ft_atof(char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		handle_error(void);

#endif
