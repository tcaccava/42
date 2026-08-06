/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleccia <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 16:30:12 by gleccia           #+#    #+#             */
/*   Updated: 2026/06/16 16:30:23 by gleccia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

int	ft_printf(const char *str, ...);

int	ft_putchar(char c);
int	ft_putstr(char *str);
int	ft_putunsigned(unsigned int n);
int	ft_puthex(unsigned int n, char format);
int	ft_putptr(void *ptr);
int	ft_putnbr_base(int nbr, char *base, int len);

#endif
