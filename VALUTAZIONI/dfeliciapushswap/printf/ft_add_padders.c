/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_padders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 17:13:42 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/04 15:31:26 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	pointerzeros(t_flags flags, int n, int len)
{
	while ((flags.zeros > 0 && flags.width > 1 && flags.minun == 0)
		&& n < (flags.width - len + 2))
		n += write (1, "0", 1);
	return (n);
}
