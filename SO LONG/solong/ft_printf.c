/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:26:37 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/03 15:52:28 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	ft_formatspecifier(char specifier, va_list args, int *length)
{
	if (specifier == 'c')
		ft_putchar(va_arg(args, int), length);
	else if (specifier == 's')
		ft_putstr(va_arg(args, char *), length);
	else if (specifier == 'd' || specifier == 'i')
		ft_putnbr(va_arg(args, int), length);
	else if (specifier == 'x')
		ft_puthexnbr(va_arg(args, unsigned int), length, 0);
	else if (specifier == 'X')
		ft_puthexnbr(va_arg(args, unsigned int), length, 1);
	else if (specifier == '%')
		ft_putchar('%', length);
	else if (specifier == 'p')
		ft_putpointer(va_arg(args, void *), length);
	else if (specifier == 'u')
		ft_putunsignednbr(va_arg(args, unsigned int), length);
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
