/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dd-alo <dd-alo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 18:06:22 by dd-alo            #+#    #+#             */
/*   Updated: 2026/11/05 18:31:48 by dd-alo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_char(char c)
{
	return (write(1, &c, 1));
}

int	ft_print_str(const char *s)
{
	int		i;

	i = 0;
	if (!s)
		s = "(null)";
	while (s[i])
	{
		if (ft_print_char(s[i]) != -1)
			i++;
		else
			return (-1);
	}
	return (i);
}

int	ft_print_base(unsigned long n, const char *base)
{
	unsigned long	base_len;
	int				i;

	base_len = ft_strlen(base);
	i = 0;
	if (n >= base_len)
	{
		i = ft_print_base(n / base_len, base);
		if (i == -1)
			return (-1);
	}
	if (ft_print_char(base[n % base_len]) == -1)
		return (-1);
	return (i + 1);
}

int	ft_print_nbr(int n)
{
	long	num;
	int		i;
	int		is_neg;

	is_neg = 0;
	num = n;
	if (num < 0)
	{
		if (ft_print_char('-') == -1)
			return (-1);
		num *= -1;
		is_neg = 1;
	}
	i = ft_print_base(num, "0123456789");
	if (i == -1)
		return (-1);
	return (i + is_neg);
}

int	ft_print_ptr(void *ptr)
{
	int		i;
	int		digits;

	i = 0;
	if (!ptr)
		return (ft_print_str("(nil)"));
	else
		i += ft_print_str("0x");
	if (i == -1)
		return (-1);
	digits = ft_print_base((unsigned long)ptr, "0123456789abcdef");
	if (digits == -1)
		return (-1);
	return (i + digits);
}
