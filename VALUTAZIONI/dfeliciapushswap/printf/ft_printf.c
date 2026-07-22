/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:20:24 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/02 15:51:16 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include "libftprintf.h"

int	ft_printf(const char *par, ...)
{
	va_list	ap;
	t_flags	flags;
	int		count;
	int		i;

	i = 0;
	count = 0;
	va_start(ap, par);
	while (par[i] != '\0')
	{
		if (par[i] == '%')
		{
			flags = parser(&par[i + 1], &i);
			i++;
			count += checker(par[i], &ap, flags);
		}
		else
			count += write(1, &par[i], 1);
		i++;
	}
	va_end (ap);
	return (count);
}
