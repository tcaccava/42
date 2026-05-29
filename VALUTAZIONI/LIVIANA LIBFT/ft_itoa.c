/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldei-sva <ldei-sva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 22:32:26 by ldei-sva          #+#    #+#             */
/*   Updated: 2024/11/25 21:01:23 by ldei-sva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	strcreate(int n, char *number, int index)
{
	char	c;

	c = (n % 10) + '0';
	if (n > 9)
		strcreate(n / 10, number, index - 1);
	number[index] = c;
}

char	*ft_itoa(int n)
{
	char	number[20];
	char	*result;
	int		index;
	int		nb;

	index = 0;
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
	{
		number[index] = '-';
		n *= -1;
		index++;
	}
	nb = n;
	while (nb > 9)
	{
		index++;
		nb /= 10;
	}
	strcreate(n, number, index);
	number[index + 1] = '\0';
	result = ft_strdup(number);
	return (result);
}

/*int main(int ac, char **av)
{
	printf("%s", ft_itoa(-1234567890));
}*/
