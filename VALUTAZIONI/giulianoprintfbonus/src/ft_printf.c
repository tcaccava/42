/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 14:56:00 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 19:15:44 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"
#include "../include/ft_printf_bonus.h"

int	putnbr(unsigned long nb, long unsigned int base, char spec)
{
	int	total;

	total = 0;
	if (nb >= base)
		total += putnbr(nb / base, base, spec);
	if (spec == 'd' || spec == 'i' || spec == 'u')
		total += write(1, &"0123456789"[nb % base], 1);
	else if (spec == 'p' || spec == 'x')
		total += write(1, &"0123456789abcdef"[nb % base], 1);
	else
		total += write(1, &"0123456789ABCDEF"[nb % base], 1);
	return (total);
}

int	is_specifier(const int c)
{
	return ((c == 'c' || c == 's' || c == 'p' || c == 'd' || c == 'i')
		|| c == 'u' || c == 'x' || c == 'X' || c == '%');
}

static int	format_parser(const char *format, va_list list, int *total)
{
	size_t	i;
	int		count;

	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%')
			found_percent(&i, format, &count, list);
		else
			count = write(1, &format[i], 1);
		if (count < 0)
			return (-1);
		*total += count;
		i++;
	}
	return (0);
}

static int	is_format_last_char_percent(const char *format)
{
	size_t	i;

	i = 0;
	while (format[i])
		i++;
	if (i > 1 && format[i - 1] == '%' && format[i - 2] != '%')
		return (1);
	if (i == 1 && format[0] == '%')
		return (1);
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	list;
	int		total;

	va_start(list, format);
	if (!format)
		return (va_end(list), -1);
	total = 0;
	if (is_format_last_char_percent(format))
		return (va_end(list), -1);
	if (format_parser(format, list, &total) == -1)
		return (va_end(list), -1);
	return (va_end(list), total);
}
