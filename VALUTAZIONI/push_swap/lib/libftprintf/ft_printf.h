/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pspaccas <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:36:55 by pspaccas          #+#    #+#             */
/*   Updated: 2025/03/06 12:37:16 by pspaccas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <limits.h>
# include <stdarg.h>
# include <unistd.h>

int		ft_printf(const char *print, ...);
void	ft_putchar(char c, int *len);
void	ft_puthex(size_t n, int *len, int up);
void	ft_putnbr(int n, int *l);
void	ft_putpointer(void *pointer, int *len);
void	ft_putstr(char *s, int *len);
void	ft_putunsigned(unsigned int n, int *len);

#endif
