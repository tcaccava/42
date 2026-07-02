/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printfhex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcalleja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 18:33:12 by mcalleja          #+#    #+#             */
/*   Updated: 2026/06/12 18:33:17 by mcalleja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_puthex(unsigned long n, const char format)
{
	int	len;

	len = 0;
	if (n >= 16)
	{
		len += ft_puthex(n / 16, format);
		len += ft_puthex(n % 16, format);
	}
	else
	{
		if (n <= 9)
			len += ft_putchar_len(n + '0');
		else
		{
			if (format == 'x')
				len += ft_putchar_len(n - 10 + 'a');
			if (format == 'X')
				len += ft_putchar_len(n - 10 + 'A');
		}
	}
	return (len);
}

int	ft_printfhex(unsigned long n, const char format)
{
	if (n == 0)
		return (ft_putchar_len('0'));
	return (ft_puthex(n, format));
}
