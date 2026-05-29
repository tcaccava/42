/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbrhex_x.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:43:50 by mpierant          #+#    #+#             */
/*   Updated: 2025/01/27 19:43:52 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_printf.h"

void	ft_init_x(char *b)
{
	int	i;

	i = 0;
	while (i < 10)
	{
		b[i] = i + '0';
		i++;
	}
	while (i < 16)
	{
		b[i] = 'a' + i - 10;
		i++;
	}
}

int	ft_putnbrhex_x(unsigned int nbr)
{
	char	a[10];
	int		i;
	int		count;
	char	b[16];

	ft_init_x(b);
	count = 0;
	if (nbr == 0)
		count += write(1, "0", 1);
	i = 0;
	while (nbr > 0)
	{
		a[i++] = b[nbr % 16];
		nbr /= 16;
	}
	i--;
	while (i >= 0)
		count += write(1, &a[i--], 1);
	return (count);
}
