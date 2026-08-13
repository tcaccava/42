/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunsignednbr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 10:44:37 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/03 10:08:17 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putunsignednbr(unsigned int n, int *length)
{
	if (n >= 10)
		ft_putunsignednbr(n / 10, length);
	ft_putchar('0' + (n % 10), length);
}
