/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_len.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcalleja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 18:08:38 by mcalleja          #+#    #+#             */
/*   Updated: 2026/06/12 18:40:05 by mcalleja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putstr_len(char *str)
{
	if (!str)
	{
		ssize_t b = write(1, "(null)", 6);
		if (b == -1) // TODO errore
		return b;
		return (6);
	}
	write(1, str, ft_strlen(str));
	return (ft_strlen(str));
}
