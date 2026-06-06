/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   conv_spec_parser_bonus.c                           :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:48:25 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 17:57:07 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_bonus.h"

static void	conv_spec_parser(
	t_flags	flags,
	t_f_data	f_data,
	va_list	list,
	int		*count)
{
	if (flags.conv_spec == 'c')
		print_char(va_arg(list, int), flags, count);
	else if (flags.conv_spec == 's')
		print_string(va_arg(list, char *), flags, count);
	else if (flags.conv_spec == 'p')
		print_address(va_arg(list, void *), &flags, &f_data, count);
	else if (flags.conv_spec == 'd' || flags.conv_spec == 'i')
		print_int(va_arg(list, int), &flags, &f_data, count);
	else if (flags.conv_spec == 'u')
		print_unsigned_int(va_arg(list, unsigned int), &flags, &f_data, count);
	else if (flags.conv_spec == 'x' || flags.conv_spec == 'X')
		print_hex(va_arg(list, unsigned int), &flags, &f_data, count);
	else if (flags.conv_spec == '%')
		*count += write(1, "%", 1);
}

void	found_percent(size_t *i, const char *format, int *count, va_list list)
{
	t_flags		flags;
	t_f_data	f_data;

	(*i)++;
	reset_flags(&flags);
	set_f_data(&f_data);
	*count = fill_flags(&flags, &format[*i]);
	*i += *count;
	*count = 0;
	conv_spec_parser(flags, f_data, list, count);
}
