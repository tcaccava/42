/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_other.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:22:56 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/02 15:51:28 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"
#include "libft.h"
#include <stdio.h>

int	checker(char c, va_list *ap, t_flags flags)
{
	if (c == 'c')
		return (printer_c(ap, flags));
	else if (c == 's')
		return (printer_s(ap, flags));
	else if (c == 'd')
		return (printer_d(ap, flags));
	else if (c == 'p')
		return (printer_p(ap, flags));
	else if (c == 'i')
		return (printer_i(ap, flags));
	else if (c == 'u')
		return (printer_u(ap, flags));
	else if (c == 'x')
		return (printer_x(ap, flags));
	else if (c == 'X')
		return (printer_upx(ap, flags));
	else if (c == '%')
		return (printer_percent(flags));
	return (0);
}

int	flagchecker(char c)
{
	if (c != 'c' && c != 's' && c != 'd' && c != 'p' && c != 'i'
		&& c != 'u' && c != 'x' && c != 'X' && c != '%')
		return (1);
	return (0);
}

t_flags	flagger(const char *par, int *i, t_flags parse, int j)
{
	while (flagchecker(par[j]) == 1)
	{
		if (par[j] == '-')
			parse.minun++;
		if (par[j] == '.')
			parse.precision = 0;
		if (par[j] == '#')
			parse.prefix++;
		if (par[j] == ' ')
			parse.spaces++;
		if (par[j] == '+')
			parse.plusle++;
		if (par[j] == '0' && parse.width == 0 && parse.precision < 0)
			parse.zeros = 1;
		else if (par[j] >= '0' && par[j] <= '9')
		{
			if (parse.precision >= 0)
				parse.precision = (parse.precision * 10) + (par[j] - '0');
			else
				parse.width = (parse.width * 10) + (par[j] - '0');
		}
		j++;
	}
	*i += j;
	return (parse);
}

t_flags	parser(const char *par, int *i)
{
	t_flags		parse;
	int			j;

	ft_memset(&parse, 0, sizeof(t_flags));
	parse.precision = -1;
	j = 0;
	parse = flagger (par, i, parse, j);
	return (parse);
}
