/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:06:33 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/04 15:12:35 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"
#include "libft.h"

int	printer_c(va_list *ap, t_flags flags)
{
	int	i;
	int	n;

	i = va_arg(*ap, int);
	n = 0;
	if (flags.width > 1 && flags.minun == 0)
	{
		while ((flags.width - 1) > n)
			n += write(1, " ", 1);
		return (n + (write(1, &i, 1)));
	}
	if (flags.width > 1 && flags.minun > 0)
	{
		n += write (1, &i, 1);
		while (flags.width > n)
			n += write(1, " ", 1);
		return (n);
	}
	return (write(1, &i, 1));
}

int	printer_s(va_list *ap, t_flags flags)
{
	char			*s;
	int				i;
	int				n;

	n = 0;
	s = va_arg(*ap, char *);
	if (!s)
		return (write(1, "(null)", 6));
	i = 0;
	while ((flags.width > 0 && flags.minun == 0)
		&& n < (flags.width - (int)ft_strlen(s)))
		n += write (1, " ", 1);
	while (s[i] && (flags.precision == -1 || i < flags.precision))
	{
		write(1, &s[i], 1);
		i++;
	}
	n += i;
	while ((flags.width > 0 && flags.minun == 1)
		&& n < flags.width)
		n += write(1, " ", 1);
	return (n);
}

int	printer_d(va_list *ap, t_flags flags)
{
	char	*s;
	int		i;
	int		n;

	s = ft_itoa(va_arg(*ap, int));
	i = 0;
	n = 0;
	n = widthpadding(flags, s, n, i);
	if (s[0] != '-' && flags.plusle == 1)
		n += write(1, "+", 1);
	else if (s[0] != '-' && flags.spaces == 1)
		n += write(1, " ", 1);
	if (s[i] == '-')
		i += write(1, "-", 1);
	n = zerospadding(flags, s, n);
	while (s[i])
	{
		write(1, &s[i], 1);
		i++;
	}
	n = widthpadding(flags, s, n, i);
	free (s);
	return (i + n);
}

int	printer_i(va_list *ap, t_flags flags)
{
	return (printer_d(ap, flags));
}

int	printer_u(va_list *ap, t_flags flags)
{
	char	*s;
	int		i;
	int		n;

	s = utoa(ap);
	i = 0;
	n = 0;
	n = widthpadding(flags, s, n, i);
	n = zerospadding(flags, s, n);
	while (s[i])
	{
		write(1, &s[i], 1);
		i++;
	}
	n = widthpadding(flags, s, n, i);
	free(s);
	return (i + n);
}
