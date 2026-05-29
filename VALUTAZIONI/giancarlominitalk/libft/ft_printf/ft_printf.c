/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 18:17:51 by girizzi           #+#    #+#             */
/*   Updated: 2025/05/20 19:11:41 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	which_format(va_list args, const char format)
{
	int	count;

	count = 0;
	if (format == 'c')
		count += ft_print_char(va_arg(args, int));
	else if (format == 's')
		count += ft_print_str(va_arg(args, char *));
	else if (format == 'p')
		count += ft_print_ptr(va_arg(args, void *));
	else if (format == 'd' || format == 'i')
		count += ft_print_nbr(va_arg(args, int));
	else if (format == 'u')
		count += ft_print_unsigned(va_arg(args, unsigned int));
	else if (format == 'x' || format == 'X')
		count += ft_print_hex(va_arg(args, unsigned int), format == 'X');
	else if (format == '%')
		count += ft_print_char('%');
	else
		return (-1);
	return (count);
}

int	ft_print_char(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_printf(const char *format, ...)
{
	int		count;
	int		i;
	va_list	args;

	count = 0;
	i = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			count += which_format(args, format[i + 1]);
			i++;
		}
		else if (format[i] == '%' && !format[i + 1])
			return (-1);
		else
			count += ft_print_char(format[i]);
		i++;
	}
	va_end(args);
	return (count);
}
