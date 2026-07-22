/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_converters.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 16:49:33 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/02 15:49:40 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	hexlonglen(unsigned long ap)
{
	int	len;

	len = 1;
	while (ap >= 16)
	{
		ap = ap / 16;
		len++;
	}
	return (len);
}

int	hexlongconverter(unsigned long ap)
{
	char	*base;
	int		i;

	i = 0;
	base = "0123456789abcdef";
	if (ap >= 16)
		i += hexlongconverter((ap / 16));
	i += write(1, &base[ap % 16], 1);
	return (i);
}
