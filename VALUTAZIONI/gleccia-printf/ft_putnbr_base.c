/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleccia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 16:35:55 by gleccia           #+#    #+#             */
/*   Updated: 2026/06/16 21:11:24 by gleccia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

#define BASE_10 "0123456789"
#define BASE_16_LOW "0123456789abcdef"
#define BASE_16_UP "0123456789ABCDEF"

int	ft_putnbr_base(int nbr, char *base, int len)
{
	long	n;
	int		count;

	count = 0;
	n = nbr;
	if (n < 0)
	{
		count += write(1, "-", 1);
		n = -n;
	}
	if (n >= len)
		count += ft_putnbr_base(n / len, base, len);
	count += write(1, &base[n % len], 1);
	return (count);
}
