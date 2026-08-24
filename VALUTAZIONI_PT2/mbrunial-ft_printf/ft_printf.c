/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunial <mbrunial@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 18:26:20 by mbrunial          #+#    #+#             */
/*   Updated: 2026/08/24 19:01:33 by mbrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

static int	printf_switch_3(char specifier, va_list args, ssize_t *counter)
{
	if (specifier == 'p')
	{
		if (write(1, "0x", 2) == -1
			|| ft_putnbr_base_uns((unsigned long long)va_arg(args, void*),
				BASE_HEXA_DOWN, LEN_HEXA, counter) == -1)
			return (-1);
		(*counter) += 2;
	}
	return (0);
}

static int	printf_switch_2(char specifier, va_list args, ssize_t *counter)
{
	if (specifier == 'd' || specifier == 'i')
	{
		if (ft_putnbr_base_sig((long long)va_arg(args, int), BASE_DECIMAL,
				LEN_DECIMAL, counter) == -1)
			return (-1);
	}
	else if (specifier == 'u')
	{
		if (ft_putnbr_base_uns((unsigned long long)va_arg(args, unsigned int),
				BASE_DECIMAL, LEN_DECIMAL, counter) == -1)
			return (-1);
	}
	else if (specifier == 'x')
	{
		if (ft_putnbr_base_uns((unsigned long long)va_arg(args, unsigned int),
				BASE_HEXA_DOWN, LEN_HEXA, counter))
			return (-1);
	}
	else if (specifier == 'X')
	{
		if (ft_putnbr_base_uns((unsigned long long)va_arg(args, unsigned int),
				BASE_HEXA_UP, LEN_HEXA, counter))
			return (-1);
	}
	return (0);
}

static int	printf_switch_1(char specifier, va_list args, ssize_t *counter)
{
	printf("entra1");
	if (specifier == 'c')
	{
		if (write(1, &(char){(char)va_arg(args, int)}, 1) == -1)
			return (-1);
		(*counter)++;
	}
	else if (specifier == '%')
	{
		if (write(1, "%", 1) == -1)
			return (-1);
		(*counter)++;
	}
	else if (specifier == 's')
	{
		if (print_str(va_arg(args, char *), counter) == -1)
			return (-1);
	}
	else if (specifier == 'a')
	{
		printf("entra");
		int a = 0;
		a = write(1, "ciao", 4);
		if (a == -1)
			return (-1);
		(*counter) += 4;
	}
	return (0);
}

int	ft_printf(const char *s, ...)
{
	va_list	args;
	ssize_t	counter;

	counter = 0;
	va_start(args, s);
	while (*s)
	{
		if (*s == '%' && *(s + 1) != '\0')
		{
			if (printf_switch_1(*(s + 1), args, &counter) == -1)
				return (-1);
			else if (printf_switch_2(*(s + 1), args, &counter) == -1)
				return (-1);
			else if (printf_switch_3(*(s + 1), args, &counter) == -1)
				return (-1);
			s++;
		}
		else if (write(1, s, 1) == -1)
			return (-1);
		else
			counter++;
		s++;
	}
	va_end(args);

	return ((int)counter);
}


