/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxiliary_functions_2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 18:11:39 by edveneto          #+#    #+#             */
/*   Updated: 2026/06/19 18:27:37 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_u(t_params *p, unsigned long long num, t_flags *f, char *base)
{
	int	count;

	count = 0;
	if (f->minus == 0)
	{
		while (p->padding-- > 0)
			count += write(1, " ", 1);
	}
	if (p->prefix[0] != '\0')
		count += write(1, p->prefix, ft_strlen(p->prefix));
	while (p->zeros-- > 0)
		count += write(1, "0", 1);
	if (p->len > 0)
		ft_print_absolute(num, base);
	count += p->len;
	if (f->minus == 1)
	{
		while (p->padding-- > 0)
			count += write(1, " ", 1);
	}
	return (count);
}

void	ft_init_params(t_params *params)
{
	params->padding = 0;
	params->zeros = 0;
	params->sign = 0;
	params->len = 0;
	params->prefix = "";
}

void	ft_set_params(t_params *p, long long num, t_flags *f, char *base)
{
	unsigned long long	u_num;
	int					p_l;

	u_num = (unsigned long long)num;
	if (f->type == 'd' || f->type == 'i')
	{
		if (num < 0)
			u_num = -num;
		if (num < 0 || f->plus || f->space)
			p->sign = 1;
	}
	p->len = ft_num_len(u_num, base);
	if (f->dot && !f->precision && !u_num)
		p->len = 0;
	if (f->dot && f->precision > p->len)
		p->zeros = f->precision - p->len;
	p_l = ft_strlen(p->prefix);
	if (f->width > (p->len + p->zeros + p->sign + p_l))
		p->padding = f->width - p->len - p->zeros - p->sign - p_l;
	if (f->zero && !f->minus && !f->dot)
	{
		p->zeros += p->padding;
		p->padding = 0;
	}
}
