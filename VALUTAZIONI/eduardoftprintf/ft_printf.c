/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epandele <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 09:54:20 by epandele          #+#    #+#             */
/*   Updated: 2025/07/10 07:05:41 by epandele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdarg.h>
#include <unistd.h>

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		char_count;

	char_count = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1))
		{
			format++;
			char_count += take_format(format, args);
		}
		else
		{
			ft_putchar(*format);
			char_count++;
		}
		format++;
	}
	va_end(args);
	return (char_count);
}

int	take_format(const char *format, va_list args)
{
	int	char_count;

	char_count = 0;
	if (*format == 'd' || *format == 'i')
		char_count += print_int(va_arg(args, int));
	else if (*format == 's')
		char_count += print_string(va_arg(args, const char *));
	else if (*format == 'c')
	{
		ft_putchar(va_arg(args, int));
		char_count++;
	}
	else if (*format == 'x')
		char_count += print_hex(va_arg(args, unsigned int), 0);
	else if (*format == 'X')
		char_count += print_hex(va_arg(args, unsigned int), 1);
	else if (*format == 'p')
		char_count += print_pointer(va_arg(args, void *));
	else if (*format == 'u')
		char_count += print_unsigned(va_arg(args, unsigned int));
	else
	{
		char_count += ft_putchar('%');
	}
	return (char_count);
}

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}
