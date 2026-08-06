/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleccia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 21:14:51 by gleccia           #+#    #+#             */
/*   Updated: 2026/06/16 21:14:55 by gleccia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_print_ptr(unsigned long n)
{
	char	*hex;
	int		count;

	hex = "0123456789abcdef";
	count = 0;
	if (n >= 16)
		count += ft_print_ptr(n / 16);
	count += ft_putchar(hex[n % 16]);
	return (count);
}

int	ft_putptr(void *ptr)
{
	int				count;
	unsigned long	addr;

	if (!ptr)
		return (write(1, "(nil)", 5));
	addr = (unsigned long)ptr;
	count = write(1, "0x", 2);
	count += ft_print_ptr(addr);
	return (count);
}
