/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_low_hex.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibottice <ibottice@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:58:38 by ibottice          #+#    #+#             */
/*   Updated: 2025/01/08 16:11:23 by ibottice         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_count_low_hex(unsigned int hex, int *count)
{
	unsigned long long	n;

	n = hex;
	ft_count_putnbr_base(n, LOW_HEX, count);
}
