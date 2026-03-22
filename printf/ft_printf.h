/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcaccava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 10:11:49 by tcaccava          #+#    #+#             */
/*   Updated: 2025/01/03 16:03:25 by tcaccava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <unistd.h>
# include <limits.h>

int		ft_printf(const char *print, ...);
void	ft_putchar(char c, int *length);
void	ft_puthexnbr(size_t n, int *length, int uppercase);
void	ft_putnbr(int n, int *length);
void	ft_putpointer(void *ptr, int *length);
void	ft_putstr(char *s, int *length);
void	ft_putunsignednbr(unsigned int n, int *length);

#endif
