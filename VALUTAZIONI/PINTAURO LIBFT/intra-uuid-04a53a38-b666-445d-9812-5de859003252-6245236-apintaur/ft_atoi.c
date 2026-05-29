/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 22:50:18 by apintaur          #+#    #+#             */
/*   Updated: 2024/12/13 07:34:10 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	value;

	i = 0;
	value = 0;
	sign = 1;
	if (nptr != NULL)
	{
		while ((nptr[i] >= 9 && nptr[i] <= 13)
			|| (nptr[i] == 32))
			i++;
		if (nptr[i] == '+' || nptr[i] == '-')
		{
			if (nptr[i] == '-')
				sign = -1;
			i++;
		}
		while (nptr[i] >= '0' && nptr[i] <= '9')
		{
			value = value * 10 + (nptr[i] - '0');
			i++;
		}
	}
	return (value * sign);
}
