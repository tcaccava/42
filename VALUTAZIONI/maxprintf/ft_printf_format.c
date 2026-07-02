/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcalleja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 17:19:05 by mcalleja          #+#    #+#             */
/*   Updated: 2026/06/12 18:55:51 by mcalleja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_format(va_list args, const char format)
{
	if (format == 'c')
		return (ft_putchar_len(va_arg(args, int)));
	else if (format == 's')
		return (ft_putstr_len(va_arg(args, char *)));
	else if (format == 'p')
		return (ft_printfptr(va_arg(args, void *)));
	else if (format == 'd' || format == 'i')
		return (ft_printfnbr(va_arg(args, int)));
	else if (format == 'u')
		return (ft_printfunbr(va_arg(args, unsigned int)));
	else if (format == 'x' || format == 'X')
		return (ft_printfhex(va_arg(args, unsigned int), format));
	else if (format == '%')
		return (ft_putchar_len('%'));
	else if (format == 'a')
		return (ft_putstr_len("hola elis"));
	return (0);
}
