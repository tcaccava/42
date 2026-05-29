/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_formatter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibottice <ibottice@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:31:39 by ibottice          #+#    #+#             */
/*   Updated: 2025/01/08 18:18:41 by ibottice         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_formatter(const char c, int *count, va_list args)
{
	if (c == 'c')
		ft_count_putchar((char)va_arg(args, int), count);
	if (c == 'd' || c == 'i')
		ft_count_putint(va_arg(args, int), count);
	if (c == 's')
		ft_count_putstr(va_arg(args, char *), count);
	if (c == 'u')
		ft_count_unint(va_arg(args, unsigned int), count);
	if (c == 'x')
		ft_count_low_hex(va_arg(args, unsigned int), count);
	if (c == 'X')
		ft_count_up_hex(va_arg(args, unsigned int), count);
	if (c == 'p')
		ft_count_ptr(va_arg(args, void *), count);
	if (c == '%')
		ft_count_putchar('%', count);
}
