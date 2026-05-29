/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimeoni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:28:57 by nsimeoni          #+#    #+#             */
/*   Updated: 2025/02/11 19:18:44 by nsimeoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stddef.h>
#include <unistd.h>

void	ft_pointr(void *ptr, int *l)
{
	size_t	pointer;
	char	string[20];
	int		i;
	char	*charbase;

	if (ptr == NULL)
	{
		write(1, "(nil)", 5);
		(*l) += 5;
		return ;
	}
	pointer = (size_t)ptr;
	charbase = "0123456789abcdef";
	i = 0;
	write(1, "0x", 2);
	(*l) += 2;
	while (pointer != 0)
	{
		string[i++] = charbase[pointer % 16];
		pointer = pointer / 16;
	}
	while (i--)
	{
		ft_putchar(string[i], l);
	}
}
