/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   specifier_printer.c                               :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:16:22 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 15:43:35 by gipaglie        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"

long long	ft_abs_base(long long nb)
{
	if (nb < 0)
		nb *= -1;
	return (nb);
}

int	print_string_base(const char *str)
{
	unsigned int	i;

	if (!str)
		return (write(1, "(null)", 6));
	i = 0;
	while (str[i])
		i++;
	return (write(1, str, i));
}

int	print_decimal_base(int nb)
{
	int	minus;

	minus = 0;
	if (nb < 0)
	{
		minus = 1;
		write(1, "-", 1);
	}
	return (minus + putnbr(ft_abs_base(nb), 10, 'd'));
}

int	print_address_base(void *ptr)
{
	int	bytes;

	bytes = 0;
	if (!ptr)
		return (write(1, "(nil)", 5));
	bytes += write(1, "0x", 2);
	bytes += putnbr((unsigned long) ptr, 16, 'p');
	return (bytes);
}
