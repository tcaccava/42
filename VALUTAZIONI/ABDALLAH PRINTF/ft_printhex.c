/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 19:03:40 by abkhefif          #+#    #+#             */
/*   Updated: 2025/01/08 19:04:15 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_printhex(unsigned int n, const char type)
{
	char	*hex;

	hex = "0123456789abcdef";
	if (!type)
		return ;
	if (n >= 16)
		ft_printhex(n / 16, type);
	if (type == 'X')
		ft_putchar(ft_toupperr(hex[n % 16]));
	else
		ft_putchar(hex[n % 16]);
}
