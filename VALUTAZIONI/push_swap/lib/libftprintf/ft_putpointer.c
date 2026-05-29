/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putpointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pspaccas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:47:37 by pspaccas          #+#    #+#             */
/*   Updated: 2025/03/06 12:47:41 by pspaccas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putpointer(void *pointer, int *len)
{
	size_t	n;

	if (!pointer)
	{
		ft_putstr("(nil)", len);
		return ;
	}
	n = (size_t)pointer;
	ft_putstr("0x", len);
	ft_puthex(n, len, 0);
}
