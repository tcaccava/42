/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keiestre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 15:39:35 by keiestre          #+#    #+#             */
/*   Updated: 2026/07/06 20:12:56 by keiestre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	j;
	int	k;
	int	res;

	j = 0;
	k = 1;
	res = 0;
	while (nptr[j] == ' ' || (nptr[j] >= 9 && nptr[j] <= 13))
		j++;
	if (nptr[j] == '+' || nptr[j] == '-')
	{
		if (nptr[j] == '-')
			k = -k;
		j++;
	}
	while (nptr[j] >= 48 && nptr[j] <= 57)
	{
		res = res * 10 + (nptr[j] - '0');
		j++;
	}
	return (res * k);
}
