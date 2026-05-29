/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pspaccas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:48:10 by pspaccas          #+#    #+#             */
/*   Updated: 2025/03/06 12:48:18 by pspaccas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putunsigned(unsigned int n, int *len)
{
	if (n >= 10)
		ft_putunsigned(n / 10, len);
	ft_putchar('0' + (n % 10), len);
}
