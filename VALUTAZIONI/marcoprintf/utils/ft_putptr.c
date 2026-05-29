/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:44:20 by mpierant          #+#    #+#             */
/*   Updated: 2025/01/27 19:44:21 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/ft_printf.h"

int	ft_putptr(void *ptr)
{
	char				a[20];
	int					i;
	int					count;
	char				b[16];
	long unsigned int	nbr;

	ft_init_x(b);
	count = 0;
	if (ptr == NULL)
		return (write(1, "(nil)", 5));
	nbr = (long unsigned int)ptr;
	count += write(1, "0x", 2);
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
