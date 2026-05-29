/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsimeoni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:29:51 by nsimeoni          #+#    #+#             */
/*   Updated: 2025/02/11 18:23:19 by nsimeoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>

void	ft_puthex(unsigned int x, int *length, char x_or_x)
{
	char	string[9];
	char	*possible;
	int		i;

	if (x_or_x == 'X')
		possible = "0123456789ABCDEF";
	else
		possible = "0123456789abcdef";
	i = 0;
	if (x == 0)
	{
		write(1, "0", 1);
		*length += 1;
		return ;
	}
	while (x != 0)
	{
		string[i] = possible[x % 16];
		x = x / 16;
		i++;
	}
	while (i--)
		ft_putchar(string[i], length);
}
