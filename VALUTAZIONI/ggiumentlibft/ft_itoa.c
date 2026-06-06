/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiument <ggiument@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 17:25:35 by ggiument          #+#    #+#             */
/*   Updated: 2026/06/02 16:24:14 by ggiument         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_malloc(long int nl)
{
	int	count;

	count = 0;
	if (nl < 0)
	{
		count++;
		nl *= -1;
	}
	while (nl > 9)
	{
		nl /= 10;
		count++;
	}
	count++;
	return (count);
}

char	*ft_itoa(int n)
{
	long int	nl;
	int			count;
	char		*str;

	nl = n;
	count = count_malloc(nl);
	str = malloc(count + 1);
	if (!str)
		return (NULL);
	if (nl < 0)
	{
		str[0] = '-';
		nl *= -1;
	}
	str[count--] = '\0';
	while (nl > 9)
	{
		str[count--] = nl % 10 + '0';
		nl /= 10;
	}
	str[count] = nl + '0';
	return (str);
}
