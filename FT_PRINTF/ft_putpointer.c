/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putpointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:03:09 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/03 15:40:37 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putpointer(void *ptr, int *length)
{
	size_t	n;

	if (!ptr)
	{
		ft_putstr("(nil)", length);
		return ;
	}
	n = (size_t)ptr;
	ft_putstr("0x", length);
	ft_puthexnbr(n, length, 0);
}
