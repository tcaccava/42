/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibrunial <ibrunial@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:40:34 by ibrunial          #+#    #+#             */
/*   Updated: 2024/12/18 12:45:32 by ibrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	byte_for_malloc(long long n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++;
		n = -n;
	}
	while (n > 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char		*dest;
	int			len;
	long long	ll_n;

	len = byte_for_malloc(n);
	dest = (char *)malloc(sizeof(char) * len + 1);
	if (!dest)
		return (NULL);
	dest[len--] = '\0';
	ll_n = (long long)n;
	if (ll_n < 0)
	{
		dest[0] = '-';
		ll_n = -ll_n;
	}
	if (ll_n == 0)
		dest[0] = '0';
	while (ll_n > 0)
	{
		dest[len--] = ll_n % 10 + '0';
		ll_n /= 10;
	}
	return (dest);
}

// int	main(void)
// {
// 	printf("%s\n", ft_itoa(0));
// }