/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_printers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 16:13:09 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/04 15:11:54 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	printer_x(va_list *ap, t_flags flags)
{
	int				i;
	int				n;
	int				len;
	unsigned int	val;

	val = va_arg(*ap, unsigned int);
	len = hexlen(val);
	if (flags.prefix == 1 && val != 0)
		len += 2;
	i = 0;
	n = 0;
	n = widthhexpadding(flags, n, len, i);
	if (flags.prefix == 1 && val != 0)
		n += write(1, "0x", 2);
	n = zerohexpadding(flags, n, len);
	i += hexconverter(val);
	n = widthhexpadding(flags, n, len, i);
	return (i + n);
}

int	printer_upx(va_list *ap, t_flags flags)
{
	int				i;
	int				n;
	int				len;
	unsigned int	val;

	val = va_arg(*ap, unsigned int);
	len = hexlen(val);
	i = 0;
	n = 0;
	n = widthhexpadding(flags, n, len, i);
	if (flags.prefix == 1 && val != 0)
		n += write(1, "0X", 2);
	n = zerohexpadding(flags, n, len);
	i += hexupconverter(val);
	n = widthhexpadding(flags, n, len, i);
	return (i + n);
}

int	printer_p(va_list *ap, t_flags flags)
{
	int				i;
	int				len;
	int				n;
	unsigned long	val;

	val = va_arg(*ap, unsigned long);
	if (!val)
		return (write(1, "(nil)", 5));
	len = (unsigned long)hexlonglen(val);
	i = 0;
	n = 0;
	n = pointerwidth(flags, n, len, i);
	n += write (1, "0x", 2);
	n = pointerzeros(flags, n, len);
	i += hexlongconverter(val);
	n = pointerwidth(flags, n, len, i);
	return (i + n);
}

int	printer_percent(t_flags flags)
{
	int	i;

	i = 0;
	while (flags.width > 1 && flags.minun == 0 && i < flags.width)
		i += write(1, " ", 1);
	i += write(1, "%", 1);
	while (flags.width > 1 && flags.minun == 1 && i < flags.width)
		i += write(1, " ", 1);
	return (i);
}
