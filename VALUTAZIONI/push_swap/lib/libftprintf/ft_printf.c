/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pspaccas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:42:36 by pspaccas          #+#    #+#             */
/*   Updated: 2025/03/06 12:00:52 by pspaccas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_formatspecifier(char specifier, va_list args, int *len)
{
	if (specifier == 'c')
		ft_putchar(va_arg(args, int), len);
	else if (specifier == 's')
		ft_putstr(va_arg(args, char *), len);
	else if (specifier == 'd' || specifier == 'i')
		ft_putnbr(va_arg(args, int), len);
	else if (specifier == 'x')
		ft_puthex(va_arg(args, unsigned int), len, 0);
	else if (specifier == 'X')
		ft_puthex(va_arg(args, unsigned int), len, 1);
	else if (specifier == '%')
		ft_putchar('%', len);
	else if (specifier == 'p')
		ft_putpointer(va_arg(args, void *), len);
	else if (specifier == 'u')
		ft_putunsigned(va_arg(args, unsigned int), len);
}

int	ft_printf(const char *print, ...)
{
	int		i;
	int		len;
	va_list	args;

	i = 0;
	len = 0;
	va_start(args, print);
	while (print[i])
	{
		if (print[i] == '%' && print[i + 1] != '\0')
		{
			i++;
			ft_formatspecifier(print[i], args, &len);
		}
		else
		{
			ft_putchar(print[i], &len);
		}
		i++;
	}
	va_end(args);
	return (len);
}
/*
#include <stdio.h>

int	main(void)
{
	ft_printf("ft_printf: %s\n", "Hello world!");
	printf("printf: %s\n", "Hello world!");
	ft_printf("ft_printf: %d\n", 42);
	printf("printf: %d\n", 42);
	ft_printf("ft_printf: %c\n", 'A');
	printf("printf: %c\n", 'A');
	ft_printf("ft_printf: %u\n", 123456);
	printf("printf: %u\n", 123456);
	ft_printf("ft_printf: %x\n", 255);
	printf("printf: %x\n", 255);
	ft_printf("ft_printf %X\n", 255);
	printf("printf %X\n", 255);
	int a = 42;
	ft_printf("ft_printf: %p\n", (void *)&a);
	printf("printf: %p\n", (void *)&a);
	ft_printf("ft_printf: %%\n");
	printf("printf: %%\n");
	return (0);
}*/