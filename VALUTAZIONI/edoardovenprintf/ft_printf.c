/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 18:11:40 by edveneto          #+#    #+#             */
/*   Updated: 2026/06/19 18:29:19 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *format, ...)
{
	int		c;
	va_list	args;

	if (!format)
		return (-1);
	c = 0;
	va_start(args, format);
	c = ft_printf_engine(format, &args);
	va_end(args);
	return (c);
}

int	ft_printf_engine(const char *format, va_list *args)
{
	int		i;
	int		print;
	t_flags	flags;

	i = 0;
	print = 0;
	while (format[i])
	{
		if (format[i] != '%')
			print += ft_print_char(format[i]);
		else
		{
			ft_init_flags(&flags);
			i = ft_parse_flags(format, &flags, i + 1);
			i = ft_parse_num(format, &flags, i);
			if (!format[i])
				break ;
			flags.type = format[i];
			print += ft_managing(args, &flags);
		}
		i++;
	}
	return (print);
}

int	ft_managing(va_list *args, t_flags *flags)
{
	int	count;

	count = 0;
	if (flags->type == 'd' || flags->type == 'i')
		count += ft_format_int(va_arg(*args, int), flags);
	else if (flags->type == 'c')
		count += ft_format_char(va_arg(*args, int), flags);
	else if (flags->type == 'x' || flags->type == 'X')
		count += ft_format_hex(va_arg(*args, unsigned int), flags);
	else if (flags->type == 'u')
		count += ft_format_uint(va_arg(*args, unsigned int), flags);
	else if (flags->type == 'p')
		count += ft_format_pointer(va_arg(*args, void *), flags);
	else if (flags->type == 's')
		count += ft_format_str(va_arg(*args, char *), flags);
	else if (flags->type == '%')
		count += ft_format_char('%', flags);
	else if (flags->type == 'a')
		count += ft_format_str("cioane", flags);
	return (count);
}
