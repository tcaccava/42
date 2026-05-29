/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilnassi <ilnassi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 19:13:46 by ilnassi           #+#    #+#             */
/*   Updated: 2025/03/27 16:57:40 by ilnassi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>
# include <stddef.h>

int		ft_atoi(const char *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	**ft_split(char const *s, char c);

/* Print Functions */
int		ft_formats(va_list args, const char format);
int		ft_printf(const char *form, ...);
int		ft_putnbr(int nb);
int		ft_putnbr_unsigned(unsigned int nb);
int		ft_print_hexa(unsigned long long nb, int uppercase);
int		ft_putstr(char *str);
int		ft_putchar(char c);
int		ft_putpercent(void);
int		ft_print_ptr(void *ptr);

#endif
