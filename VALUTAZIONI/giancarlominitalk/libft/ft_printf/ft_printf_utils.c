/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:50:22 by girizzi           #+#    #+#             */
/*   Updated: 2025/05/20 19:16:29 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_str(char *s)
{
	int	count;

	count = 0;
	if (!s)
		return (ft_print_str("(null)"));
	while (*s)
	{
		count += ft_print_char(*s);
		s++;
	}
	return (count);
}

int	ft_print_ptr(void *ptr)
{
	unsigned long	tmp;
	int				count;

	count = 0;
	if (ptr == NULL)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	tmp = (unsigned long)ptr;
	count += write(1, "0x", 2);
	count += ft_print_hex(tmp, 0);
	return (count);
}

int	ft_print_nbr(int n)
{
	int	count;

	count = 0;
	if (n == -2147483648)
	{
		ft_print_str("-2147483648");
		return (11);
	}
	if (n < 0)
	{
		ft_print_char('-');
		n = -n;
		count++;
	}
	if (n >= 10)
		count += ft_print_nbr(n / 10);
	ft_print_char(n % 10 + '0');
	count++;
	return (count);
}

int	ft_print_unsigned(unsigned int n)
{
	int	count;

	count = 0;
	if (n == 0)
	{
		ft_print_char('0');
		return (1);
	}
	if (n >= 10)
		count += ft_print_unsigned(n / 10);
	ft_print_char(n % 10 + '0');
	count++;
	return (count);
}

int	ft_print_hex(unsigned long num, int uppercase)
{
	const char	*base;
	int			count;

	if (num == 0)
	{
		ft_print_char('0');
		return (1);
	}
	if (uppercase)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	count = 0;
	if (num >= 16)
		count += ft_print_hex(num / 16, uppercase);
	ft_print_char(base[num % 16]);
	count++;
	return (count);
}
