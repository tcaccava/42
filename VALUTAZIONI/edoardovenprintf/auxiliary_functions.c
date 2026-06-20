/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxiliary_functions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 18:11:39 by edveneto          #+#    #+#             */
/*   Updated: 2026/06/19 18:19:38 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_str(char *str, int length)
{
	int	c;

	c = 0;
	while (*str && length > 0)
	{
		write(1, str, 1);
		c++;
		str++;
		length--;
	}
	return (c);
}

int	ft_print_char(char c)
{
	write(1, &c, 1);
	return (1);
}

void	ft_print_absolute(unsigned long long num, char *base)
{
	int	base_len;

	base_len = ft_strlen(base);
	if (num >= (unsigned long long)base_len)
		ft_print_absolute(num / base_len, base);
	write(1, &base[num % base_len], 1);
	return ;
}

int	ft_num_len(unsigned long long num, char *base)
{
	int					c;
	unsigned long long	base_len;

	c = 1;
	base_len = ft_strlen(base);
	if (num >= base_len)
		c += ft_num_len(num / base_len, base);
	return (c);
}

int	ft_print_num(t_params *p, long int num, t_flags *f)
{
	int	count;

	count = 0;
	if (f->minus == 0)
		while (p->padding-- > 0)
			count += write(1, " ", 1);
	if (p->sign == 1 && num < 0)
		count += write(1, "-", 1);
	else if (p->sign == 1 && f->plus == 1)
		count += write(1, "+", 1);
	else if (p->sign == 1)
		count += write(1, " ", 1);
	if (num < 0)
		num = -num;
	while (p->zeros-- > 0)
		count += write(1, "0", 1);
	if (p->len > 0)
		ft_print_absolute(num, "0123456789");
	count += p->len;
	if (f->minus == 1)
		while (p->padding-- > 0)
			count += write(1, " ", 1);
	return (count);
}
