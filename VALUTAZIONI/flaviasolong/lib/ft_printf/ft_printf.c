/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: faiello <faiello@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 23:06:21 by faiello           #+#    #+#             */
/*   Updated: 2025/10/16 23:29:09 by faiello          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	print_str(const char *s)
{
	int	len;

	if (!s)
		return (write(1, "(null)", 6));
	len = 0;
	while (s[len])
		len++;
	return (write(1, s, len));
}

static int	print_signed(long n)
{
	char	buf[20];
	int		i;
	int		written;

	if (n == 0)
		return (write(1, "0", 1));
	written = 0;
	if (n < 0)
	{
		written += write(1, "-", 1);
		n = -n;
	}
	i = 0;
	while (n > 0)
	{
		buf[i++] = (char)('0' + (n % 10));
		n /= 10;
	}
	while (i-- > 0)
		written += write(1, &buf[i], 1);
	return (written);
}

static int	print_unsigned(unsigned long n)
{
	char	buf[20];
	int		i;
	int		written;

	if (n == 0)
		return (write(1, "0", 1));
	i = 0;
	while (n > 0)
	{
		buf[i++] = (char)('0' + (n % 10));
		n /= 10;
	}
	written = 0;
	while (i-- > 0)
		written += write(1, &buf[i], 1);
	return (written);
}

static int	handle_format(char spec, va_list args)
{
	char	c;

	if (spec == 'c')
	{
		c = (char)va_arg(args, int);
		return (write(1, &c, 1));
	}
	if (spec == 's')
		return (print_str(va_arg(args, const char *)));
	if (spec == 'd' || spec == 'i')
		return (print_signed(va_arg(args, int)));
	if (spec == 'u')
		return (print_unsigned(va_arg(args, unsigned int)));
	if (spec == '%')
		return (write(1, "%", 1));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		total;
	int		i;

	if (!format)
		return (0);
	va_start(args, format);
	total = 0;
	i = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			total += handle_format(format[i], args);
		}
		else
			total += write(1, &format[i], 1);
		i++;
	}
	va_end(args);
	return (total);
}
