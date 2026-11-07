/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dd-alo <dd-alo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 19:32:29 by dd-alo            #+#    #+#             */
/*   Updated: 2026/07/28 19:06:08 by dd-alo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	intcounter(long n)
{
	int	count;

	count = 1;
	while (n > 9)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static void	fill_str(char *str, long num, int len, int neg)
{
	len--;
	while (len >= neg)
	{
		str[len] = num % 10 + '0';
		num /= 10;
		len--;
	}
	if (neg)
		str[0] = '-';
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		neg;
	long	num;

	neg = 0;
	num = n;
	if (num < 0)
	{
		num *= -1;
		neg++;
	}
	len = neg + intcounter(num);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = 0;
	fill_str(str, num, len, neg);
	return (str);
}
