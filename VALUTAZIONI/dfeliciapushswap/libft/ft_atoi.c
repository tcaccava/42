/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 19:19:18 by mde-matt          #+#    #+#             */
/*   Updated: 2026/05/20 12:24:59 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <stdio.h>*/

int	ft_atoi(const char *nptr)
{
	int	i;
	int	minus;
	int	num;

	i = 0;
	minus = 0;
	num = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			minus++;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (minus == 0)
			num = (num * 10) + (nptr[i] - 48);
		else
			num = (num * 10) - (nptr[i] - 48);
		i++;
	}
	return (num);
}
/*
int	main()
{
	printf("%d\n", ft_atoi("2147483648"));
}*/
