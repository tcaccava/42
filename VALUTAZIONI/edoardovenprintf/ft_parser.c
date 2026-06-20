/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 18:11:40 by edveneto          #+#    #+#             */
/*   Updated: 2026/06/19 18:11:40 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_init_flags(t_flags	*flags)
{
	flags->minus = 0;
	flags->plus = 0;
	flags->hash = 0;
	flags->space = 0;
	flags->zero = 0;
	flags->dot = 0;
	flags->width = 0;
	flags->precision = 0;
	flags->type = '\0';
}

int	ft_parse_flags(const char *format, t_flags *flags, int i)
{
	while (format[i])
	{
		if (format[i] == '-')
			flags->minus = 1;
		else if (format[i] == '+')
			flags->plus = 1;
		else if (format[i] == '#')
			flags->hash = 1;
		else if (format[i] == ' ')
			flags->space = 1;
		else if (format[i] == '0')
			flags->zero = 1;
		else
			return (i);
		i++;
	}
	return (i);
}

int	ft_parse_num(const char *format, t_flags *flags, int i)
{
	while (ft_isdigit(format[i]))
	{
		flags->width *= 10;
		flags->width += format[i] - '0';
		i++;
	}
	if (format[i] == '.')
	{
		flags->dot = 1;
		i++;
	}
	else
		return (i);
	while (ft_isdigit(format[i]))
	{
		flags->precision *= 10;
		flags->precision += format[i] - '0';
		i++;
	}
	return (i);
}
