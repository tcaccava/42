/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:26:37 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/19 20:17:22 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_formatspecifier(char specifier, va_list args, int *length)
{
	if (specifier == 'd' || specifier == 'i')
		ft_putnbr(va_arg(args, int), length);
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

void	ft_putnbr(int n, int *length)
{
	if (n == INT_MIN)
	{
		ft_putstr("-2147483648", length);
		return ;
	}
	if (n < 0)
	{
		ft_putchar('-', length);
		n = -n;
	}
	if (n >= 10)
		ft_putnbr((n / 10), length);
	ft_putchar('0' + (n % 10), length);
}

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
