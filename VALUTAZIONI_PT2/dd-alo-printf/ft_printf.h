/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dd-alo <dd-alo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 18:22:41 by dd-alo            #+#    #+#             */
/*   Updated: 2026/11/05 19:24:23 by dd-alo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include "libft/libft.h"

int	ft_print_char(char c);
int	ft_print_str(const char *s);
int	ft_print_base(unsigned long n, const char *base);
int	ft_print_nbr(int n);
int	ft_print_ptr(void *ptr);
int	ft_print_format(char spec, va_list *args);
int	ft_printf(const char *format, ...);

#endif
