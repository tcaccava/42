/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 10:11:26 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/03 16:02:45 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

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
