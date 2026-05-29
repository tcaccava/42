/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimeoni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:28:10 by nsimeoni          #+#    #+#             */
/*   Updated: 2025/02/11 18:20:43 by nsimeoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int		ft_printf(const char *format, ...);
void	ft_putchar(int c, int *lenght);
void	ft_putcharacter_length(char c, int *length);
void	ft_putstr(char *str, int *length);
void	ft_putnbr(int n, int *length);
void	ft_unsigned_int(unsigned int n, int *length);
void	ft_puthex(unsigned int n, int *length, char x_or_x);
void	ft_pointr(void *ptr, int *length);

#endif
