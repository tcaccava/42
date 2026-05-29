/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibottice <ibottice@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:23:19 by ibottice          #+#    #+#             */
/*   Updated: 2025/01/10 17:40:34 by ibottice         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

# define DECIMAL "0123456789"
# define LOW_HEX "0123456789abcdef"
# define UP_HEX "0123456789ABCDEF"

int		ft_printf(const char *str, ...);

size_t	ft_strlen(const char *str);

void	ft_count_low_hex(unsigned int hex, int *count);
void	ft_count_up_hex(unsigned int hex, int *count);
void	ft_count_ptr(void *ptr, int *count);
void	ft_formatter(const char c, int *count, va_list args);
void	ft_count_putstr(char *str, int *count);
void	ft_count_putnbr_base(unsigned long long i, char *base, int *count);
void	ft_count_putchar(const char c, int *count);
void	ft_count_putint(int n, int *count);
void	ft_count_unint(unsigned int n, int *count);
#endif
