/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:26:37 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/13 11:13:27 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_formatspecifier(char specifier, va_list args, int *length)
{
	if (specifier == 'c')
		ft_putchar(va_arg(args, int), length);
	else if (specifier == 's')
		ft_putstr(va_arg(args, char *), length);
}

int	ft_printf(const char *print, ...)
{
	int		i;
	int		length;
	va_list	args;

	i = 0;
	length = 0;
	va_start(args, print);
	while (print[i])
	{
		if (print[i] == '%' && print[i + 1] != '\0')
		{
			i++;
			ft_formatspecifier(print[i], args, &length);
		}
		else
		{
			ft_putchar(print[i], &length);
		}
		i++;
	}
	va_end(args);
	return (length);
}
/*
#include <stdio.h>

int	main(void)
{
	int	a;

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
	a = 42;
	ft_printf("ft_printf: %p\n", (void *)&a);
	printf("printf: %p\n", (void *)&a);
	ft_printf("ft_printf: %%\n");
	printf("printf: %%\n");
	return (0);
}
*/

void	ft_putchar(char c, int *length)
{
	write(1, &c, 1);
	(*length)++;
}

void	ft_putstr(char *s, int *length)
{
	if (!s)
		s = "(null)";
	while (*s)
	{
		ft_putchar(*s, length);
		s++;
	}
}
