/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edveneto <edveneto@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 18:11:40 by edveneto          #+#    #+#             */
/*   Updated: 2026/06/19 18:32:25 by edveneto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

/* LIBRERIE*/
# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft/libft.h"

typedef struct s_flags
{
	int		minus;
	int		plus;
	int		hash;
	int		zero;
	int		dot;
	int		width;
	int		space;
	int		precision;
	char	type;
}	t_flags;

typedef struct s_params
{
	int		padding;
	int		zeros;
	int		sign;
	int		len;
	char	*prefix;
}	t_params;

/* PROTOTIPI DELLE FUNZIONI */

/* ft_printf.c */
int		ft_printf(const char *format, ...);
int		ft_printf_engine(const char *format, va_list *args);
int		ft_managing(va_list *args, t_flags *flags);

/* ft_parser.c */
void	ft_init_flags(t_flags *flags);
int		ft_parse_flags(const char *format, t_flags *flags, int i);
int		ft_parse_num(const char *format, t_flags *flags, int i);

/* format_numbers.c */
int		ft_format_int(long int num, t_flags *flags);
int		ft_format_uint(unsigned int num, t_flags *flags);

/* format_hex.c */
int		ft_format_hex(unsigned int num, t_flags *flags);
int		ft_format_pointer(void *ptr, t_flags *flags);

/* format_text.c */
int		ft_format_char(int c, t_flags *flags);
int		ft_format_str(char *str, t_flags *flags);

/* auxiliary_functions.c */
int		ft_print_str(char *str, int length);
int		ft_print_char(char c);
void	ft_print_absolute(unsigned long long num, char *base);
int		ft_num_len(unsigned long long num, char *base);
int		ft_print_num(t_params *p, long int num, t_flags *f);

/* auxiliary_functions_2.c */
int		ft_print_u(t_params *p, unsigned long long num, t_flags *f, char *base);
void	ft_init_params(t_params *p);
void	ft_set_params(t_params *p, long long num, t_flags *f, char *base);

#endif
