/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abkhefif <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 19:21:59 by abkhefif          #+#    #+#             */
/*   Updated: 2025/01/08 19:28:08 by abkhefif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_printf(const char *format, ...);
int		ft_lenhex(unsigned int nbr);
int		ft_lenptr(unsigned long nbr);
int		ft_lenunbr(unsigned int nbr);
int		is_format(char c, va_list argptr);
void	ft_printptr(unsigned long n);
int		ft_putchar(char c);
int		ft_puthex(unsigned int nbr, char type);
int		ft_putnbr(int i);
int		ft_putptr(void *ptr);
int		ft_putstr(char *str);
int		ft_putunbr(unsigned int n);
int		ft_putnbr_fd(int n, int fd);
int		ft_putstr_fd(char *s, int fd);
void	ft_printhex(unsigned int n, const char type);
int		ft_putchar_fd(char c, int fd);
int		ft_isdigitt(int c);
int		ft_toupperr(int c);
char	*ft_itoaa(int n);

#endif
