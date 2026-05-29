/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 16:01:04 by valucare          #+#    #+#             */
/*   Updated: 2026/03/07 14:06:26 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

t_complex	sum_complex(t_complex c1, t_complex c2)
{
	t_complex	res;

	res.re = c1.re + c2.re;
	res.im = c1.im + c2.im;
	return (res);
}

t_complex	square_complex(t_complex c)
{
	t_complex	res;

	res.re = (c.re * c.re) - (c.im * c.im);
	res.im = 2 * c.re * c.im;
	return (res);
}
