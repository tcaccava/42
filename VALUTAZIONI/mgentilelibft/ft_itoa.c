/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgentile <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 15:38:01 by mgentile          #+#    #+#             */
/*   Updated: 2026/06/04 19:51:06 by mgentile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*string(int n, char *result, int size)
{
	int	tmp;

	tmp = 0;
	tmp = n;
	if (tmp < 0)
		tmp = -tmp;
	while (tmp > 0)
	{
		size--;
		result[size] = tmp % 10 + '0';
		tmp /= 10;
	}
	if (n < 0)
		result[0] = '-';
	return (result);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		size;
	int		tmp;

	if (n == 0)
		return (ft_strdup("0"));
	if (n == INT_MIN)
		return (ft_strdup("-2147483648"));
	size = 0;
	tmp = n;
	if (tmp < 0)
	{
		size++;
		tmp = -tmp;
	}
	while (tmp != 0)
	{
		tmp /= 10;
		size++;
	}
	result = malloc(size + 1);
	if (!result)
		return (NULL);
	result[size] = '\0';
	return (string(n, result, size));
}

/*int main()
{
	// Test 1: numero positivo
	printf("Test 1: 123\n");
	printf("  risultato: %s\n", ft_itoa(123));

	// Test 2: numero negativo
	printf("\nTest 2: -456\n");
	printf("  risultato: %s\n", ft_itoa(-456));

	// Test 3: zero
	printf("\nTest 3: 0\n");
	printf("  risultato: %s\n", ft_itoa(0));

	// Test 4: INT_MAX
	printf("\nTest 4: INT_MAX (2147483647)\n");
	printf("  risultato: %s\n", ft_itoa(INT_MAX));

	// Test 5: INT_MIN
	printf("\nTest 5: INT_MIN (-2147483648)\n");
	printf("  risultato: %s\n", ft_itoa(INT_MIN));

	// Test 6: numero a una cifra
	printf("\nTest 6: 7\n");
	printf("  risultato: %s\n", ft_itoa(7));

	// Test 7: numero negativo a una cifra
	printf("\nTest 7: -7\n");
	printf("  risultato: %s\n", ft_itoa(-7));

	return (0);
}*/
