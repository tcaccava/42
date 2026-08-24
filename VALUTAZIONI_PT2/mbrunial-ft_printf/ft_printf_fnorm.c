/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fnorm.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunial <mbrunial@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 18:47:36 by mbrunial          #+#    #+#             */
/*   Updated: 2026/08/24 18:51:17 by mbrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	print_str(const char *s, ssize_t *counter)
{
	ssize_t	len;
	ssize_t	tmp;

	if (!s)
		return (-1);
	len = 0;
	while (s[len])
		len++;
	tmp = write(1, s, len);
	if (tmp == -1)
		return (-1);
	else
		(*counter) += tmp;
	return (0);
}

ssize_t	ft_putnbr_base_uns(unsigned long long num, char *base,
	size_t base_len, ssize_t *counter)
{
	if (num >= base_len)
		ft_putnbr_base_uns((num / base_len), base, base_len, counter);
	if (write(1, (base + (num % base_len)), 1) == -1)
		return (-1);
	else
		(*counter)++;
	return (0);
}

ssize_t	ft_putnbr_base_sig(long long num, char *base, ssize_t base_len,
	ssize_t *counter)
{
	if (num < 0)
	{
		num = -num;
		(*counter) += write(1, "-", 1);
	}
	if (num >= base_len)
		ft_putnbr_base_sig(num / base_len, base, base_len, counter);
	if (write(1, (base + (num % base_len)), 1) == -1)
		return (-1);
	else
		(*counter)++;
	return (0);
}
