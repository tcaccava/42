/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:48:44 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 18:13:14 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_bonus.h"

void	putnchar(unsigned char c, int nb, int *count)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		i++;
		*count += write(1, &c, 1);
	}
}

void	print_literal_notation(t_f_data *f_data, int *count, int case_sensitive)
{
	f_data->literal_notation = (char *) malloc(2 * sizeof(char));
	if (!f_data)
		return ;
	f_data->literal_notation[0] = '0';
	if (!case_sensitive)
		f_data->literal_notation[1] = 'x';
	else
		f_data->literal_notation[1] = 'X';
	*count += write(1, f_data->literal_notation, 2);
	free(f_data->literal_notation);
}

void	print_padding_literalnotation(
	int validated,
	t_flags	flags,
	t_f_data	*f_data,
	int	*count)
{
	if (flags.left_align || flags.zero)
	{
		if (validated)
			print_literal_notation(f_data, count, flags.conv_spec == 'X');
		putnchar(f_data->padding, f_data->padding_len, count);
	}
	else
	{
		putnchar(f_data->padding, f_data->padding_len, count);
		if (validated)
			print_literal_notation(f_data, count, flags.conv_spec == 'X');
	}
}
