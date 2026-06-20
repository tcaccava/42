/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_numbers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 18:11:39 by edveneto          #+#    #+#             */
/*   Updated: 2026/06/19 18:31:46 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_format_int(long int num, t_flags *flags)
{
	int			count;
	t_params	params;

	count = 0;
	ft_init_params(&params);
	ft_set_params(&params, num, flags, "0123456789");
	count += ft_print_num(&params, num, flags);
	return (count);
}

int	ft_format_uint(unsigned int num, t_flags *flags)
{
	int			count;
	t_params	params;

	count = 0;
	ft_init_params(&params);
	ft_set_params(&params, num, flags, "0123456789");
	count += ft_print_u(&params, num, flags, "0123456789");
	return (count);
}
