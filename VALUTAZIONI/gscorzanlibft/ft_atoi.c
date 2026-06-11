/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gscorzon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 17:14:11 by gscorzon          #+#    #+#             */
/*   Updated: 2026/06/08 14:56:07 by gscorzon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_sign(char nptr)
{
	if (nptr == '-')
		return (-1);
	else if (nptr == '+')
		return (+1);
	else if (!(nptr >= 48 && nptr <= 57))
		return (0);
	return (+1);
}

static int	skipspace(char *nptr)
{
	int	i;

	i = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == ' ')
	{
		nptr++;
		i++;
	}
	return (i);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	j;
	int	sign;

	i = 0;
	j = 0;
	nptr += skipspace((char *)nptr);
	sign = ft_sign(*nptr);
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	while (*nptr)
	{
		if (*nptr >= 48 && *nptr <= 57)
		{
			i = *nptr - 48;
			j = (j * 10) + i;
			nptr++;
		}
		else
			return (j * sign);
	}
	return (j * sign);
}
