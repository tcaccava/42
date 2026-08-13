/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexnbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 11:46:22 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/03 16:26:31 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

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
