/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierant <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 19:39:15 by mpierant          #+#    #+#             */
/*   Updated: 2025/01/27 19:40:11 by mpierant         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int		ft_printf(const char *format, ...);
int		ft_print_format(const char specifier, va_list ap);

int		ft_putchar(int c);
int		ft_putstr(char *str);
int		ft_putnbr_u(unsigned int nbr);
int		ft_putnbr(int n);
void	ft_init_x(char *b);
int		ft_putnbrhex_x(unsigned int nbr);
int		ft_putptr(void *ptr);
void	ft_init_X(char *b);
int		ft_putnbrhex_xcapital(unsigned int nbr);

#endif
