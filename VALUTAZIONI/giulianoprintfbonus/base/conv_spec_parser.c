/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   conv_spec_parser.c                                 :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:05:45 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 15:05:45 by gipaglie        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

static int	specifier_parser(const char spec, va_list list)
{
	unsigned char	c;

	if (spec == 'c')
	{
		c = (unsigned char) va_arg(list, int);
		return (write(1, &c, 1));
	}
	else if (spec == 's')
		return (print_string_base(va_arg(list, char *)));
	else if (spec == 'p')
		return (print_address_base(va_arg(list, void *)));
	else if (spec == 'd' || spec == 'i')
		return (print_decimal_base(va_arg(list, int)));
	else if (spec == 'u')
		return (putnbr(ft_abs_base(va_arg(list, unsigned int)), 10, spec));
	else if (spec == 'x' || spec == 'X')
		return (putnbr(ft_abs_base(va_arg(list, unsigned int)), 16, spec));
	else if (spec == '%')
		return (write(1, "%", 1));
	return (-1);
}

void	found_percent(size_t *i, const char *format, int *count, va_list list)
{
	(*i)++;
	*count = 0;
	*count = specifier_parser(format[*i], list);
}
