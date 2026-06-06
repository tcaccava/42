/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   specifier_c_s.c                                   :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:48:47 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 17:12:38 by gipaglie        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_bonus.h"

void	print_char(unsigned char c, t_flags flags, int *count)
{
	unsigned char	padding;

	padding = ' ';
	if (flags.zero)
		padding = '0';
	if (flags.width && !flags.left_align)
		putnchar(padding, flags.width - 1, count);
	*count += write(1, &c, 1);
	if (flags.width && flags.left_align)
		putnchar(padding, flags.width - 1, count);
}

static void	ft_putstr(char *str, t_flags flags, int *count, int i)
{
	if (!str)
	{
		if (flags.precision <= 0 || flags.precision > 5)
			*count += write(1, "(null)", i);
	}
	else
		*count += write(1, str, i);
}

void	print_string(char *str, t_flags flags, int *count)
{
	int	i;

	i = 0;
	if (!str)
	{
		if (flags.precision <= 0 || flags.precision > 5)
			i = 6;
	}
	else
	{
		while (str[i])
			i++;
		if (flags.precision == 0)
			return ;
		if (i >= flags.precision && flags.precision > 0)
			i = flags.precision;
	}
	if (flags.width > i && !flags.left_align)
		putnchar(' ', flags.width - i, count);
	ft_putstr(str, flags, count, i);
	if (flags.width > i && flags.left_align)
		putnchar(' ', flags.width - i, count);
}
