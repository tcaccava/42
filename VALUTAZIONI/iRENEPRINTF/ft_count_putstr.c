/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_putstr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibottice <ibottice@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:25:02 by ibottice          #+#    #+#             */
/*   Updated: 2025/01/08 18:12:49 by ibottice         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_count_putstr(char *str, int *count)
{
	if (!str)
	{
		ft_count_putstr("(null)", count);
		return ;
	}
	while (*str)
	{
		ft_count_putchar(*str, count);
		str++;
	}
}
