/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvovlas <lvovlas@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 20:19:14 by lvovlas           #+#    #+#             */
/*   Updated: 2026/07/18 13:00:55 by lvovlas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	dig_count(unsigned int n)
{
	size_t	d;

	d = 1;
	while (n >= 10)
	{
		n /= 10;
		d++;
	}
	return (d);
}

static int	is_negative(int n, unsigned int *un)
{
	if (n < 0)
	{
		*un = (unsigned int)(-(long)n);
		return (1);
	}
	*un = (unsigned int)n;
	return (0);
}

char	*ft_itoa(int n)
{
	unsigned int	un;
	int				negative;
	size_t			digits;
	size_t			len;
	char			*str;

	negative = is_negative(n, &un);
	digits = dig_count(un);
	len = digits + (unsigned int)negative + 1;
	str = (char *)malloc(len * sizeof(char));
	if (!str)
		return (NULL);
	str[len - 1] = '\0';
	while (digits--)
	{
		str[digits + ((unsigned int)negative)] = (char)('0' + (un % 10));
		un /= 10;
	}
	if (negative)
		str[0] = '-';
	return (str);
}
