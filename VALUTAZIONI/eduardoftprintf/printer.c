/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epandele <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:37:38 by epandele          #+#    #+#             */
/*   Updated: 2025/07/10 07:19:19 by epandele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_int(int i)
{
	int	char_count;

	char_count = 0;
	if (i < 0)
	{
		ft_putchar('-');
		char_count++;
		char_count += print_unsigned((unsigned int)(-i));
	}
	else
	{
		char_count += print_unsigned((unsigned int)i);
	}
	return (char_count);
}

int	print_string(const char *str)
{
	int	char_count;

	char_count = 0;
	if (str == NULL)
	{
		str = "(null)";
	}
	while (*str)
	{
		char_count += ft_putchar(*str);
		str++;
	}
	return (char_count);
}

int	print_pointer(void *ptr)
{
	unsigned long	address;
	int				char_count;

	address = (unsigned long)ptr;
	char_count = 0;
	if (ptr == NULL)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	ft_putchar('0');
	ft_putchar('x');
	char_count += 2;
	char_count += print_hex(address, 0);
	return (char_count);
}

int	print_unsigned(unsigned int val)
{
	int	char_count;

	char_count = 0;
	if (val >= 10)
	{
		char_count += print_unsigned(val / 10);
	}
	char_count += ft_putchar('0' + (val % 10));
	return (char_count);
}

int	print_hex(unsigned long value, int uppercase)
{
	int		char_count;
	char	digit;

	char_count = 0;
	if (value >= 16)
	{
		char_count += print_hex(value / 16, uppercase);
	}
	if ((value % 16) < 10)
	{
		digit = '0' + (value % 16);
	}
	else
	{
		if (uppercase)
			digit = 'A' + ((value % 16) - 10);
		else
			digit = 'a' + ((value % 16) - 10);
	}
	char_count += ft_putchar(digit);
	return (char_count);
}
