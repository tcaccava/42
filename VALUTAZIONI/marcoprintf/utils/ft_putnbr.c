/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:43:39 by mpierant          #+#    #+#             */
/*   Updated: 2025/01/27 19:43:41 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_printf.h"

int	ft_putnbr(int n)
{
	unsigned int	nbr;
	char			a[10];
	int				i;
	int				count;

	count = 0;
	if (n == 0)
		return (write(1, "0", 1));
	if (n < 0)
	{
		count += write(1, "-", 1);
		nbr = -n;
	}
	if (n > 0)
		nbr = n;
	i = 0;
	while (nbr > 0)
	{
		a[i++] = nbr % 10 + '0';
		nbr /= 10;
	}
	i--;
	while (i >= 0)
		count += write(1, &a[i--], 1);
	return (count);
}
