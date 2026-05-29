/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <tcaccava@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:26:57 by ibottice          #+#    #+#             */
/*   Updated: 2025/04/24 17:08:29 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *text, ...)
{
	va_list	args;
	int		count;
	int		i;

	count = 0;
	i = 0;
	va_start(args, text);
	while (text[i] != '\0')
	{
		if (text[i] == '%' && text[i + 1])
		{
			i++;
			ft_formatter(text[i], &count, args);
		}
		else
			ft_count_putchar(text[i], &count);
		i++;
	}
	va_end(args);
	return (count);
}
