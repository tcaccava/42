/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                   +:+ +:+         +:+      */
/*   By: gipaglie <marvin@42.fr>                   #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/06/04 15:48:59 by gipaglie         #+#    #+#              */
/*   Updated: 2026/06/04 18:29:13 by gipaglie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdio.h>
# include <stdarg.h>

void		found_percent(
				size_t *i,
				const char *format,
				int *count,
				va_list list);
int			ft_printf(const char *format, ...);
int			putnbr(unsigned long nb, long unsigned int base, char spec);
int			is_specifier(const int c);
int			print_address_base(void *ptr);
int			print_decimal_base(int nb);
int			print_string_base(const char *str);
long long	ft_abs_base(long long nb);

#endif
