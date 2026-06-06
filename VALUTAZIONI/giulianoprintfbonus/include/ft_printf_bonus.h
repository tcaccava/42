/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_printf_bonus.h                                  :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:48:56 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 18:35:34 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_BONUS_H
# define FT_PRINTF_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdarg.h>
# include <stdlib.h>
# define ND -1
# define TRUE 1
# define FALSE 0

typedef struct s_flags
{
	int		left_align;
	int		zero;
	int		dot;
	int		width;
	int		precision;
	int		plus;
	int		space;
	int		hash;
	char	conv_spec;
}	t_flags;

typedef struct s_f_data
{
	char			*literal_notation;
	int				negative_arg;
	int				precision_len;
	int				len;
	int				padding_len;
	unsigned char	padding;
}	t_f_data;

int			digit_len(unsigned long long nb, int base);
void		exec_if_negative_nb(int negative_nb, t_flags *flags);
int			exec_if_nb_zero(
				unsigned long long nb,
				t_flags flags,
				unsigned char padding,
				int *count);
void		exec_if_sign(
				int plus,
				int negative_arg,
				int *count);
void		exec_if_zero(
				int zero,
				unsigned char *padding,
				int *negative_arg,
				int *count);
int			fill_flags(t_flags *flags, const char *format);
void		fill_f_data(t_flags *flags, t_f_data *f_data, int nb_len);
void		found_percent(
				size_t *i,
				const char *format,
				int *count,
				va_list list);
int			ft_printf(const char *format, ...);
long long	ft_abs(long long nb, t_f_data *f_data);
int			ft_isdigit(const int c);
int			is_specifier(const int c);
void		print_address(
				void *ptr,
				t_flags *flags,
				t_f_data *f_data,
				int *count);
void		print_char(unsigned char c, t_flags flags, int *count);
void		print_int(
				long long nb,
				t_flags *flags,
				t_f_data *f_data,
				int *count);
void		print_hex(
				long long nb,
				t_flags *flags,
				t_f_data *f_data,
				int *count);
void		print_literal_notation(
				t_f_data *f_data,
				int *count,
				int case_sensitive);
void		print_padding_literalnotation(
				int validated,
				t_flags flags,
				t_f_data *f_data,
				int *count);
void		print_string(char *str, t_flags flags, int *count);
void		print_unsigned_int(
				long long nb,
				t_flags *flags,
				t_f_data *f_data,
				int *count);
int			putnbr_long_decimal(long nb);
void		putnchar(unsigned char c, int nb, int *count);
int			putnbr(unsigned long nb, long unsigned int base, char spec);
void		reset_flags(t_flags *flags);
void		set_f_data(t_f_data *f_data);
#endif
