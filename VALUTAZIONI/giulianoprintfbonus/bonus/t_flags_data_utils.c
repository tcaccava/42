/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   t_flags_data_utils.c                               :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:48:53 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 18:14:35 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_bonus.h"

void	fill_f_data(t_flags *flags, t_f_data *f_data, int nb_len)
{
	if (flags->precision > nb_len)
		f_data->precision_len = flags->precision - nb_len;
	f_data->len = flags->plus + f_data->negative_arg
		+ nb_len + f_data->precision_len + flags->hash * 2;
	if (flags->width > f_data->len)
		f_data->padding_len = flags->width - f_data->len;
}

void	set_f_data(t_f_data *f_data)
{
	f_data->precision_len = FALSE;
	f_data->len = 0;
	f_data->padding_len = 0;
	f_data->literal_notation = NULL;
	f_data->negative_arg = FALSE;
}

void	reset_flags(t_flags *flags)
{
	flags->left_align = FALSE;
	flags->zero = FALSE;
	flags->dot = FALSE;
	flags->width = FALSE;
	flags->precision = ND;
	flags->plus = FALSE;
	flags->space = FALSE;
	flags->hash = FALSE;
	flags->conv_spec = '\0';
}

static void	flags_values_in_format(const char *format, t_flags *flags, int *i)
{
	if (format[*i] == '-')
		flags->left_align = TRUE;
	else if (format[*i] == '.')
	{
		flags->dot = TRUE;
		flags->precision = 0;
		while (ft_isdigit(format[*i + 1]))
		{
			(*i)++;
			flags->precision = flags->precision * 10 + (format[*i] - '0');
		}
	}
	else if (format[*i] == '0' && !flags->width)
		flags->zero = TRUE;
	else if (ft_isdigit(format[*i]))
		flags->width = flags->width * 10 + (format[*i] - '0');
	else if (format[*i] == ' ' && flags->plus == 0)
		flags->space = TRUE;
	else if (format[*i] == '+')
		flags->plus = TRUE;
	else if (format[*i] == '#')
		flags->hash = TRUE;
}

int	fill_flags(t_flags *flags, const char *format)
{
	int	i;

	i = 0;
	while (format[i] && !is_specifier(format[i]))
	{
		flags_values_in_format(format, flags, &i);
		i++;
	}
	if (flags->left_align || flags->dot)
		flags->zero = FALSE;
	if (flags->plus)
		flags->space = FALSE;
	if (is_specifier(format[i]))
		flags->conv_spec = format[i];
	return (i);
}
