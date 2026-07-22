/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_converters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:16:43 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/02 15:50:31 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

char	*utoaconverter(char *s, unsigned int a, unsigned int n)
{
	int	i;

	i = 0;
	while (a >= 1)
	{
		s[i] = (n / a) + 48;
		n = n - ((n / a) * a);
		a = a / 10;
		i++;
	}
	s[i] = '\0';
	return (s);
}

char	*utoa(va_list *ap)
{
	unsigned int	n;
	unsigned int	a;
	int				i;
	char			*s;

	a = 1;
	n = 0;
	i = 1;
	n = va_arg(*ap, unsigned int);
	while (a <= (n / 10))
	{
		a = a * 10;
		i++;
	}
	s = malloc(i + 1);
	if (!s)
		return (NULL);
	s = utoaconverter(s, a, n);
	return (s);
}

int	hexconverter(unsigned int ap)
{
	char	*base;
	int		i;

	i = 0;
	base = "0123456789abcdef";
	if (ap >= 16)
		i += hexconverter(ap / 16);
	i += write (1, &base[ap % 16], 1);
	return (i);
}

int	hexupconverter(unsigned int ap)
{
	char	*base;
	int		i;

	i = 0;
	base = "0123456789ABCDEF";
	if (ap >= 16)
		i += hexupconverter(ap / 16);
	i += write (1, &base[ap % 16], 1);
	return (i);
}

int	hexlen(unsigned int ap)
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
