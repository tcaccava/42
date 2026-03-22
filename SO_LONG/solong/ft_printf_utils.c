/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 04:07:43 by tcaccava          #+#    #+#             */
/*   Updated: 2025/02/03 04:07:44 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	ft_putchar(char c, int *length)
{
	write(1, &c, 1);
	(*length)++;
}

void	ft_puthexnbr(size_t n, int *length, int uppercase)
{
	char	*base;

	if (uppercase)
	{
		base = "0123456789ABCDEF";
	}
	else
	{
		base = "0123456789abcdef";
	}
	if (n >= 16)
	{
		ft_puthexnbr(n / 16, length, uppercase);
	}
	ft_putchar(base[n % 16], length);
}

void	ft_putnbr(int n, int *length)
{
	if (n == -2147483648)
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

void	ft_putpointer(void *ptr, int *length)
{
	size_t	n;

	if (!ptr)
	{
		ft_putstr("(nil)", length);
		return ;
	}
	n = (size_t)ptr;
	ft_putstr("0x", length);
	ft_puthexnbr(n, length, 0);
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
