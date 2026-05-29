/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 12:02:46 by abkhefif          #+#    #+#             */
/*   Updated: 2025/01/08 19:17:57 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	is_format(char c, va_list argptr)
{
	if (c == 'c')
		return (ft_putchar(va_arg(argptr, int)));
	else if (c == 's')
		return (ft_putstr(va_arg(argptr, char *)));
	else if (c == 'p')
		return (ft_putptr(va_arg(argptr, void *)));
	else if (c == 'd')
		return (ft_putnbr(va_arg(argptr, int)));
	else if (c == 'i')
		return (ft_putnbr(va_arg(argptr, int)));
	else if (c == 'u')
		return (ft_putunbr(va_arg(argptr, unsigned int)));
	else if (c == 'x')
		return (ft_puthex(va_arg(argptr, unsigned int), 'x'));
	else if (c == 'X')
		return (ft_puthex(va_arg(argptr, unsigned int), 'X'));
	else if (c == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	argptr;
	int		count;
	int		i;

	va_start(argptr, format);
	count = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			count += is_format(format[i], argptr);
		}
		else
			count += ft_putchar(format[i]);
		i++;
	}
	va_end(argptr);
	return (count);
}
