/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: girizzi <girizzi@student.42roma.it>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 17:29:16 by girizzi           #+#    #+#             */
/*   Updated: 2025/01/02 21:07:55 by girizzi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	num_digits(int n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		i++;
	while (n)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char		*str_num;
	size_t		digits;
	long int	num;

	num = n;
	digits = num_digits(n);
	if (n < 0)
	{
		num *= -1;
		digits++;
	}
	str_num = (char *)malloc((digits + 1) * (sizeof(char)));
	if (! str_num)
		return (NULL);
	str_num[digits] = '\0';
	while (digits)
	{
		digits--;
		str_num[digits] = num % 10 + '0';
		num /= 10;
	}
	if (n < 0)
		str_num[0] = '-';
	return (str_num);
}
