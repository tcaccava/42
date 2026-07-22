/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-matt <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 15:43:46 by mde-matt          #+#    #+#             */
/*   Updated: 2026/06/02 15:48:33 by mde-matt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_flags
{
	int	width;
	int	minun;
	int	zeros;
	int	precision;
	int	prefix;
	int	spaces;
	int	plusle;
}		t_flags;

int			ft_printf(const char *par, ...);
int			printer_c(va_list *ap, t_flags flags);
int			printer_s(va_list *ap, t_flags flags);
int			printer_d(va_list *ap, t_flags flags);
int			printer_i(va_list *ap, t_flags flags);
int			printer_u(va_list *ap, t_flags flags);
int			printer_x(va_list *ap, t_flags flags);
int			printer_upx(va_list *ap, t_flags flags);
int			printer_p(va_list *ap, t_flags flags);
int			printer_percent(t_flags flags);
int			checker(char c, va_list *ap, t_flags flags);
int			flagchecker(char c);
int			hexconverter(unsigned int ap);
int			hexupconverter(unsigned int ap);
int			hexlongconverter(unsigned long ap);
int			widthpadding(t_flags flags, char *s, int n, int i);
int			zerospadding(t_flags flags, char *s, int n);
int			hexlen(unsigned int ap);
int			widthhexpadding(t_flags flags, int n, int len, int i);
int			zerohexpadding(t_flags flags, int n, int len);
int			hexlonglen(unsigned long ap);
int			pointerwidth(t_flags flags, int n, int len, int i);
int			pointerzeros(t_flags flags, int n, int len);
t_flags		flagger(const char *par, int *i, t_flags parse, int j);
t_flags		parser(const char *par, int *i);
char		*utoaconverter(char *s, unsigned int a, unsigned int n);
char		*utoa(va_list *ap);

#endif
