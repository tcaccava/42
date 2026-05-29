/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_up_hex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibottice <ibottice@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:00:50 by ibottice          #+#    #+#             */
/*   Updated: 2025/01/08 16:11:45 by ibottice         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_count_up_hex(unsigned int hex, int *count)
{
	unsigned long long	n;

	n = hex;
	ft_count_putnbr_base(n, UP_HEX, count);
}
