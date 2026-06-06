/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   specifier_d_i_u.c                                  :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:48:49 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 18:15:46 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_bonus.h"

void	print_int(long long nb, t_flags *flags, t_f_data *f_data, int *count)
{
	int	nb_len;

	nb = ft_abs(nb, f_data);
	nb_len = 0;
	f_data->padding = ' ';
	if (exec_if_nb_zero(nb, *flags, f_data->padding, count))
		return ;
	exec_if_negative_nb(f_data->negative_arg, flags);
	nb_len += digit_len(nb, 10);
	fill_f_data(flags, f_data, nb_len);
	exec_if_zero(flags->zero, &f_data->padding, &f_data->negative_arg, count);
	if (flags->space)
		*count += write(1, " ", 1);
	if (!flags->left_align)
		putnchar(f_data->padding, f_data->padding_len, count);
	exec_if_sign(flags->plus, f_data->negative_arg, count);
	putnchar('0', f_data->precision_len, count);
	*count += putnbr(nb, 10, flags->conv_spec);
	if (flags->left_align)
		putnchar(f_data->padding, f_data->padding_len, count);
}

void	print_unsigned_int(
	long long nb,
	t_flags	*flags,
	t_f_data	*f_data,
	int	*count)
{
	int	nb_len;

	nb = ft_abs(nb, f_data);
	nb_len = 0;
	f_data->padding = ' ';
	if (exec_if_nb_zero(nb, *flags, f_data->padding, count))
		return ;
	exec_if_negative_nb(f_data->negative_arg, flags);
	nb_len += digit_len(nb, 10);
	fill_f_data(flags, f_data, nb_len);
	exec_if_zero(flags->zero, &f_data->padding, &f_data->negative_arg, count);
	if (flags->space)
		*count += write(1, " ", 1);
	if (!flags->left_align)
		putnchar(f_data->padding, f_data->padding_len, count);
	exec_if_sign(flags->plus, f_data->negative_arg, count);
	putnchar('0', f_data->precision_len, count);
	*count += putnbr(nb, 10, flags->conv_spec);
	if (flags->left_align)
		putnchar(f_data->padding, f_data->padding_len, count);
}
