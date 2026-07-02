/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcalleja <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:36:09 by mcalleja          #+#    #+#             */
/*   Updated: 2026/06/12 19:09:28 by mcalleja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft/libft.h"

int		ft_print_format(va_list args, const char format);
int		ft_printf(char const *str, ...);
int		ft_printfnbr(int n);
int		ft_unlen(unsigned int str);
char	*ft_utoa(unsigned int n);
int		ft_printfunbr(unsigned int n);
int		ft_putchar_len(char c);
int		ft_putstr_len(char *str);
int		ft_printfptr(void *ptr);
int		ft_printfhex(unsigned long n, const char format);
int		ft_printf_format(va_list args, const char format);

#endif
