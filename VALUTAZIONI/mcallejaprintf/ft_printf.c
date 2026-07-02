/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcalleja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 16:01:09 by mcalleja          #+#    #+#             */
/*   Updated: 2026/06/12 18:49:56 by mcalleja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(char const *str, ...)
{
	int			i;
	int			len;
	va_list		args;

	if (!str)
		return (-1);
	i = 0;
	len = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%' && str[i + 1])
		{
			len += ft_printf_format(args, str[i + 1]);
			i++;
		}
		else
			len += ft_putchar_len(str[i]);
		i++;
	}
	va_end(args);
	return (len);
}
