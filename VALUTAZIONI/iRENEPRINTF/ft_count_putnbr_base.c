/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_putnbr_base.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibottice <ibottice@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:07:21 by ibottice          #+#    #+#             */
/*   Updated: 2025/01/08 18:27:23 by ibottice         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_count_putnbr_base(unsigned long long i, char *base, int *count)
{
	size_t	len;

	len = ft_strlen(base);
	if (i >= len)
		ft_count_putnbr_base(i / len, base, count);
	ft_count_putchar(base[i % len], count);
}
