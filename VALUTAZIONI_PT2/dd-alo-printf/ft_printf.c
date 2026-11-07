/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dd-alo <dd-alo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 18:10:35 by dd-alo            #+#    #+#             */
/*   Updated: 2026/11/05 18:11:55 by dd-alo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_format(char spec, va_list *args)
{
	if (spec == 'c')
		return (ft_print_char(va_arg(*args, int)));
	if (spec == 's')
		return (ft_print_str(va_arg(*args, char *)));
	if (spec == 'd' || spec == 'i')
		return (ft_print_nbr(va_arg(*args, int)));
	if (spec == 'u')
		return (ft_print_base(va_arg(*args, unsigned int), "0123456789"));
	if (spec == 'x')
		return (ft_print_base(va_arg(*args, unsigned int), "0123456789abcdef"));
	if (spec == 'X')
		return (ft_print_base(va_arg(*args, unsigned int), "0123456789ABCDEF"));
	if (spec == 'p')
		return (ft_print_ptr(va_arg(*args, void *)));
	if (spec == '%')
		return (ft_print_char('%'));
	return (-1);
}

int	ft_printf(const char *format, ...)
{
	va_list		args;
	int			i;
	int			count;
	int			res;

	i = 0;
	count = 0;
	res = 0;
	va_start(args, format);
	while (format[i] != '\0')
	{
		if (format[i] != '%')
			count = ft_print_char(format[i]);
		else
			count = ft_print_format(format[++i], &args);
		if (count == -1)
		{
			res = -1;
			break ;
		}
		res += count;
		i++;
	}
	va_end(args);
	return (res);
}
