/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoaa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:50:44 by abkhefif          #+#    #+#             */
/*   Updated: 2025/01/08 18:52:20 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	check(long n)
{
	int	tot;

	tot = 0;
	if (n < 0)
	{
		n *= -1;
		tot++;
	}
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		tot++;
	}
	return (tot);
}

char	*ft_itoaa(int n)
{
	char	*dest;
	int		i;
	long	nb;

	nb = n;
	i = check(nb);
	dest = malloc(sizeof(char) * (i + 1));
	if (!dest)
		return (NULL);
	dest[i--] = '\0';
	if (nb == 0)
		return (dest[0] = '0', dest);
	if (nb < 0)
	{
		dest[0] = '-';
		nb = -nb;
	}
	while (nb)
	{
		dest[i--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (dest);
}
