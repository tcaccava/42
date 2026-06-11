/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscorzon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 15:59:18 by gscorzon          #+#    #+#             */
/*   Updated: 2026/06/06 13:30:28 by gscorzon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_countdigits(long int n)
{
	int	i;

	i = 0;
	if (n < 0)
	{
		i++;
		n *= -1;
	}
	if (n == 0)
		return (1);
	while (n > 0)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char		*nm;
	long int	num;
	int			i;

	i = ft_countdigits((long)n);
	nm = malloc(sizeof(char) * i + 1);
	if (!nm)
		return (NULL);
	nm[i] = '\0';
	num = n;
	if (num == 0)
		nm[0] = '0';
	if (num < 0)
	{
		num *= -1;
		nm[0] = '-';
	}
	while (num > 0)
	{
		i--;
		nm[i] = num % 10 + '0';
		num /= 10;
	}
	return (nm);
}
