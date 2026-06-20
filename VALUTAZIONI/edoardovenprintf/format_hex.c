/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 18:11:39 by edveneto          #+#    #+#             */
/*   Updated: 2026/06/19 18:28:13 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_format_hex(unsigned int num, t_flags *flags)
{
	int			count;
	char		*base;
	t_params	params;

	count = 0;
	ft_init_params(&params);
	if (flags->type == 'X')
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	if (flags->hash == 1 && num != 0)
	{
		if (flags->type == 'X')
			params.prefix = "0X";
		else
			params.prefix = "0x";
	}
	ft_set_params(&params, num, flags, base);
	count += ft_print_u(&params, num, flags, base);
	return (count);
}

int	ft_format_pointer(void *ptr, t_flags *flags)
{
	int					count;
	char				*base;
	t_params			params;
	unsigned long long	addr;

	if (!ptr)
	{
		flags->dot = 0;
		return (ft_format_str("(nil)", flags));
	}
	count = 0;
	addr = (unsigned long long)ptr;
	ft_init_params(&params);
	base = "0123456789abcdef";
	params.prefix = "0x";
	ft_set_params(&params, addr, flags, base);
	count += ft_print_u(&params, addr, flags, base);
	return (count);
}
