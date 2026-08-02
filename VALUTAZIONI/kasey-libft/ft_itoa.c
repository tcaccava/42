/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:39:35 by keiestre          #+#    #+#             */
/*   Updated: 2026/07/06 20:12:56 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*special_cases(int n)
{
	char	*res;

	if (n == 0)
	{
		res = (char *)malloc(2 * sizeof(char));
		if (res == NULL)
			return (NULL);
		res[0] = '0';
		res[1] = '\0';
		return (res);
	}
	if (n == -2147483648)
	{
		res = (char *)malloc(12 * sizeof(char));
		if (res == NULL)
			return (NULL);
		ft_strlcpy(res, "-2147483648", 12);
		return (res);
	}
	return (NULL);
}

static char	*main_ft(int n, int sign, int len)
{
	char	*res;

	res = (char *)malloc((len + sign + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);
	res[len + sign] = '\0';
	if (sign)
		res[0] = '-';
	while (n != 0)
	{
		res[len + sign - 1] = (n % 10) + '0';
		n = n / 10;
		len--;
	}
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;
	int		sign;
	int		temp;

	len = 0;
	sign = 0;
	temp = n;
	res = special_cases(n);
	if (res != NULL)
		return (res);
	if (n < 0)
	{
		sign = 1;
		n = -n;
		len++;
	}
	while (temp != 0)
	{
		temp = temp / 10;
		len++;
	}
	res = main_ft(n, sign, len);
	return (res);
}
