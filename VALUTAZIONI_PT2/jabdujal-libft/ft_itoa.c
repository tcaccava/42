/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jabdujal <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 17:16:45 by jabdujal          #+#    #+#             */
/*   Updated: 2026/08/17 18:27:24 by jabdujal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_dgtcount(long num)
{
	int	digit;

	if (!num)
		return (1);
	digit = 0;
	while (num)
	{
		digit++;
		num = num / 10;
	}
	return (digit);
}

char	*ft_itoa(int num)
{
	int		digits;
	int		sign;
	long	n;
	char	*res;

	n = num;
	digits = ft_dgtcount(n);
	sign = (n < 0);
	res = malloc((digits + 1 + sign) * sizeof(*res));
	if (!res)
		return (NULL);
	if (sign)
	{
		res[0] = '-';
		n *= -1;
		digits += sign;
	}
	res[digits] = '\0';
	while (digits > sign)
	{
		digits--;
		res[digits] = n % 10 + '0';
		n /= 10;
	}
	return (res);
}
