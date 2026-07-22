/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_padders.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 17:43:25 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/04 15:28:22 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"
#include "libft.h"
#include <stdio.h>

int	widthpadding(t_flags flags, char *s, int n, int i)
{
	int	len;

	len = (int)ft_strlen(s);
	if (flags.precision > len)
		len = flags.precision;
	while ((flags.width > 1 && flags.minun == 0)
		&& n < (flags.width - len)
		&& flags.zeros == 0)
		n += write (1, " ", 1);
	while ((flags.width > 1 && flags.minun == 1)
		&& n < (flags.width - len)
		&& flags.zeros == 0 && i != 0)
		n += write (1, " ", 1);
	return (n);
}

int	zerospadding(t_flags flags, char *s, int n)
{
	int	slen;
	int	zeros;

	zeros = 0;
	slen = (int)ft_strlen(s);
	if (flags.precision > -1 && s[0] == '-')
		slen--;
	while ((flags.zeros == 1 && flags.width > 1 && flags.minun == 0)
		&& n < (flags.width - slen))
		n += write (1, "0", 1);
	while (flags.precision > -1 && (slen + zeros) < flags.precision)
	{
		zeros += write(1, "0", 1);
		n++;
	}
	return (n);
}

int	widthhexpadding(t_flags flags, int n, int len, int i)
{
	int	truelen;

	truelen = len;
	if (flags.precision > truelen)
		truelen = flags.precision;
	while ((flags.width > 1 && flags.minun == 0)
		&& n < (flags.width - truelen)
		&& flags.zeros == 0)
		n += write (1, " ", 1);
	while ((flags.width > 1 && flags.minun == 1)
		&& n < (flags.width - truelen)
		&& flags.zeros == 0 && i != 0)
		n += write (1, " ", 1);
	return (n);
}

int	zerohexpadding(t_flags flags, int n, int len)
{
	int	zeros;
	int	truelen;

	truelen = len;
	zeros = 0;
	if (flags.precision > truelen)
		truelen = flags.precision;
	while ((flags.zeros > 0 && flags.width > 1 && flags.minun == 0)
		&& n < (flags.width - truelen))
		n += write (1, "0", 1);
	while (flags.precision > -1 && (len + zeros) < flags.precision)
	{
		zeros += write (1, "0", 1);
		n++;
	}
	return (n);
}

int	pointerwidth(t_flags flags, int n, int len, int i)
{
	while ((flags.width > 1 && flags.minun == 0)
		&& n < (flags.width - len)
		&& flags.zeros == 0 && i == 0)
		n += write(1, " ", 1);
	while ((flags.width > 1 && flags.minun == 1)
		&& n < (flags.width - len)
		&& flags.zeros == 0 && i > 0)
		n += write (1, " ", 1);
	return (n);
}
