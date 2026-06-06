/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   number_utils.c                                    :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:48:39 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 15:48:39 by gipaglie        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_bonus.h"

long long	ft_abs(long long nb, t_f_data *f_data)
{
	if (nb < 0)
	{
		nb *= -1;
		f_data->negative_arg = TRUE;
	}
	return (nb);
}

int	digit_len(unsigned long long nb, int base)
{
	int	nb_len;

	nb_len = 0;
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb /= base;
		nb_len++;
	}
	return (nb_len);
}

int	ft_isdigit(const int c)
{
	return (c >= '0' && c <= '9');
}
