/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunial <mbrunial@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 15:30:07 by mbrunial          #+#    #+#             */
/*   Updated: 2026/08/24 18:52:45 by mbrunial         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# define BASE_HEXA_UP "0123456789ABCDEF"
# define BASE_HEXA_DOWN "0123456789abcdef"
# define BASE_DECIMAL "0123456789"

# define LEN_HEXA 16
# define LEN_DECIMAL 10

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

int		ft_printf(const char *s, ...);
int		print_str(const char *s, ssize_t *counter);
ssize_t	ft_putnbr_base_uns(unsigned long long num, char *base,
			size_t base_len, ssize_t *counter);
ssize_t	ft_putnbr_base_sig(long long num, char *base, ssize_t base_len,
			ssize_t *counter);

#endif