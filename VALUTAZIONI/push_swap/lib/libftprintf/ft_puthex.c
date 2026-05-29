/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pspaccas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:47:00 by pspaccas          #+#    #+#             */
/*   Updated: 2025/03/06 12:47:04 by pspaccas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_puthex(size_t n, int *len, int up)
{
	char	*base;

	if (up)
	{
		base = "0123456789ABCDEF";
	}
	else
	{
		base = "0123456789abcdef";
	}
	if (n >= 16)
	{
		ft_puthex(n / 16, len, up);
	}
	ft_putchar(base[n % 16], len);
}
