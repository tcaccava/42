/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   specifier_p_x_X.c                                  :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:48:51 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 18:11:50 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_bonus.h"

void	print_hex(long long nb, t_flags *flags, t_f_data *f_data, int *count)
{
	int	nb_len;

	f_data->padding = ' ';
	if (exec_if_nb_zero(nb, *flags, f_data->padding, count))
		return ;
	nb_len = digit_len(nb, 16);
	exec_if_zero(flags->zero, &f_data->padding, &f_data->negative_arg, count);
	if (flags->left_align)
		print_padding_literalnotation(flags->hash, *flags, f_data, count);
	fill_f_data(flags, f_data, nb_len);
	if (!flags->left_align && nb != 0)
		print_padding_literalnotation(flags->hash, *flags, f_data, count);
	putnchar('0', f_data->precision_len, count);
	*count += putnbr(nb, 16, flags->conv_spec);
	if (flags->left_align)
		putnchar(f_data->padding, f_data->padding_len, count);
}

static void	exec_if_ptr_is_valid(t_flags *flags, int *ptr_valid)
{
	flags->hash = TRUE;
	*ptr_valid = TRUE;
}

static void	exec_if_ptr_not_valid(
	t_flags	flags,
int	*count,
int	*nb_len,
int	*ptr_valid)
{
	if (flags.left_align)
		*count = write(1, "(nil)", 5);
	*nb_len = 5;
	*ptr_valid = FALSE;
}

void	print_address(void *ptr, t_flags *flags, t_f_data *f_data, int *count)
{
	unsigned long	nb;
	int				nb_len;
	int				ptr_valid;

	if (!ptr)
		exec_if_ptr_not_valid(*flags, count, &nb_len, &ptr_valid);
	else
		exec_if_ptr_is_valid(flags, &ptr_valid);
	nb = (unsigned long) ptr;
	f_data->padding = ' ';
	if (ptr)
		nb_len = digit_len(nb, 16);
	exec_if_zero(flags->zero, &f_data->padding, &f_data->negative_arg, count);
	if (flags->left_align)
		print_padding_literalnotation(ptr_valid, *flags, f_data, count);
	fill_f_data(flags, f_data, nb_len);
	if (!flags->left_align)
		print_padding_literalnotation(ptr_valid, *flags, f_data, count);
	putnchar('0', f_data->precision_len, count);
	if (ptr)
		*count += putnbr(nb, 16, flags->conv_spec);
	else if (!ptr && !flags->left_align)
		*count += write(1, "(nil)", 5);
	if (flags->left_align)
		putnchar(f_data->padding, f_data->padding_len, count);
}
