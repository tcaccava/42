/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epandele <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 10:38:47 by epandele          #+#    #+#             */
/*   Updated: 2025/07/09 10:38:48 by epandele         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int	ft_printf(const char *format, ...);
int	take_format(const char *format, va_list args);
int	ft_putchar(char c);
int	print_int(int i);
int	print_string(const char *str);
int	print_pointer(void *ptr);
int	print_unsigned(unsigned int val);
int	print_hex(unsigned long value, int uppercase);

#endif
