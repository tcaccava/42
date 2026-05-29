/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_u.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:44:10 by mpierant          #+#    #+#             */
/*   Updated: 2025/01/27 19:44:11 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_printf.h"

int	ft_putnbr_u(unsigned int nbr)
{
	char	a[10];
	int		i;
	int		count;

	count = 0;
	if (nbr == 0)
		count += write(1, "0", 1);
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
