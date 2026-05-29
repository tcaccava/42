/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:40:50 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/05/19 14:31:19 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(long value)
{
	int	len;

	len = 0;
	if (value <= 0)
	{
		value = -value;
		len++;
	}
	while (value)
	{
		value /= 10;
		len++;
	}
	return (len);
}

static char	*fill_string(char *result, long number, int index)
{
	if (number == 0)
	{
		result[0] = '0';
		return (result);
	}
	if (number < 0)
	{
		result[0] = '-';
		number = -number;
	}
	while (number > 0)
	{
		result[index] = (number % 10) + '0';
		number /= 10;
		index--;
	}
	return (result);
}

char	*ft_itoa(int n)
{
	char	*result;
	long	nb;
	int		size;

	nb = (long)n;
	size = count_digits(nb);
	result = malloc(sizeof(char) * (size + 1));
	if (result == NULL)
		return (NULL);
	result[size] = '\0';
	return (fill_string(result, nb, size - 1));
}
