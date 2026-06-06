/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   exec_if.c                                          :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:48:36 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 18:10:34 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_bonus.h"

int	exec_if_nb_zero(
	unsigned long long nb,
	t_flags	flags,
	unsigned char padding,
	int	*count)
{
	if (flags.hash && nb == 0 && flags.precision == FALSE)
		return (1);
	if (nb == 0 && (!flags.precision
			|| ((!flags.precision || flags.precision == ND) && flags.hash)))
	{
		if (flags.width || flags.hash)
		{
			if (flags.hash)
			{
				putnchar(padding, flags.width - 1, count);
				*count += write(1, "0", 1);
			}
			else
				putnchar(padding, flags.width, count);
		}
		return (1);
	}
	return (0);
}

void	exec_if_sign(int plus, int negative_arg, int *count)
{
	if (plus)
		*count += write(1, "+", 1);
	if (negative_arg)
		*count += write(1, "-", 1);
}

void	exec_if_negative_nb(int negative_nb, t_flags *flags)
{
	if (negative_nb)
	{
		flags->plus = FALSE;
		if (flags->space)
			flags->space = FALSE;
	}
}

void	exec_if_zero(
	int zero,
	unsigned char	*padding,
	int	*negative_arg,
	int	*count)
{
	if (zero)
	{
		*padding = '0';
		if (*negative_arg)
		{
			*count += write(1, "-", 1);
			*negative_arg = FALSE;
		}
	}
}
