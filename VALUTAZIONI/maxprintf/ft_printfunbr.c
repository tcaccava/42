/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printfunbr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcalleja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:06:09 by mcalleja          #+#    #+#             */
/*   Updated: 2026/06/12 18:48:35 by mcalleja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

int	ft_unlen(unsigned int str)
{
	int	len;

	len = 0;
	while (str != 0)
	{
		len++;
		str = str / 10;
	}
	return (len);
}

char	*ft_utoa(unsigned int n)
{
	char	*str;
	int		len;

	len = ft_unlen(n);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (n != 0)
	{
		str[len - 1] = n % 10 + '0';
		n = n / 10;
		len--;
	}
	return (str);
}

int	ft_printfunbr(unsigned int n)
{
	int		len;
	char	*str;

	len = 0;
	if (n == 0)
	{
		write(1, "0", 1);
		return (1);
	}
	else
	{
		str = ft_utoa(n);
		len += ft_putstr_len(str);
		free(str);
	}
	return (len);
}
