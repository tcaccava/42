/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpapale <mpapale@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 15:43:47 by mpapale           #+#    #+#             */
/*   Updated: 2026/06/16 15:43:47 by mpapale          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
SYNOPSIS
		int atoi(const char *nptr);
DESCRIPTION
		The atoi() function converts the initial portion of the string pointed to
		by nptr to int.  The behavior is the same as
			strtol(nptr, NULL, 10);
		except that atoi() does not detect errors.
RETURN VALUE
		The converted value or 0 on error.
*/
#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	s;
	int	i;
	int	r;

	s = 0;
	i = 0;
	r = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			s += 1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		r = r * 10 + (nptr[i] - '0');
		i++;
	}
	if ((s % 2) != 0)
		r *= -1;
	return (r);
}
