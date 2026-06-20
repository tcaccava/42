/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_text.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 18:11:39 by edveneto          #+#    #+#             */
/*   Updated: 2026/06/19 18:11:40 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_format_char(int c, t_flags *flags)
{
	int	count;
	int	padding;

	count = 0;
	padding = 0;
	if (flags->width > 0)
		padding = (flags->width - 1);
	if (flags->minus == 0)
	{
		while (padding-- > 0)
			count += ft_print_char(' ');
		count += ft_print_char(c);
	}
	else
	{
		count += ft_print_char(c);
		while (padding-- > 0)
			count += ft_print_char(' ');
	}
	return (count);
}

int	ft_format_str(char *str, t_flags *flags)
{
	int	count;
	int	length;
	int	padding;

	count = 0;
	if (str == NULL)
	{
		if (flags->dot == 1 && flags->precision < 6)
			str = "";
		else
			str = "(null)";
	}
	length = ft_strlen(str);
	if (flags->dot == 1 && flags->precision < length)
		length = flags->precision;
	padding = flags->width - length;
	if (flags->minus == 0)
		while (padding-- > 0)
			count += ft_print_char(' ');
	count += ft_print_str(str, length);
	if (flags->minus == 1)
		while (padding-- > 0)
			count += ft_print_char(' ');
	return (count);
}
