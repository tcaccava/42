/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibottice <ibottice@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:15:54 by ibottice          #+#    #+#             */
/*   Updated: 2025/01/08 17:24:16 by ibottice         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_count_ptr(void *ptr, int *count)
{
	unsigned long long	n;

	if (!ptr)
		return (ft_count_putstr("(nil)", count));
	n = (unsigned long long)ptr;
	ft_count_putstr("0x", count);
	ft_count_putnbr_base(n, LOW_HEX, count);
}
