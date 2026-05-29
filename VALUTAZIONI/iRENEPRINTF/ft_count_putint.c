/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_putint.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibottice <ibottice@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:36:06 by ibottice          #+#    #+#             */
/*   Updated: 2025/01/08 18:12:16 by ibottice         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_count_putint(int n, int *count)
{
	long long	i;

	i = n;
	if (i < 0)
	{
		i = -i;
		ft_count_putchar('-', count);
	}
	ft_count_putnbr_base((unsigned long long)i, DECIMAL, count);
}
